# -*- coding: UTF-8 -*-
'''
Created on 2015-3-3

@author: hcz
'''
from component.jdb.hwy.hwy_mapping import HwyMapping
from component.jdb.hwy.hwy_mapping import HwyLinkMapping
from component.rdf.hwy.hwy_data_mng_rdf import HwyDataMngRDF
from component.jdb.hwy.hwy_ic_info import convert_tile_id
from component.jdb.hwy.hwy_def import UPDOWN_TYPE_UP
from component.rdf.hwy.hwy_def_rdf import HWY_PATH_TYPE_MAIN
from component.rdf.hwy.hwy_def_rdf import HWY_UPDOWN_TYPE_NONE
from component.rdf.hwy.hwy_def_rdf import HWY_IC_TYPE_SA
from component.rdf.hwy.hwy_def_rdf import HWY_IC_TYPE_PA
from component.rdf.hwy.hwy_def_rdf import HWY_IC_TYPE_JCT
from component.rdf.hwy.hwy_def_rdf import HWY_IC_TYPE_RAMP
from component.rdf.hwy.hwy_def_rdf import HWY_IC_TYPE_IC
HWY_INVALID_ROAD_NO = -1  # 无效道路番号
HWY_INVALID_IC_NO = 0  # 无效ic_no
HWY_ROAD_ATTR_NONE = 0


class HwyMappingRDF(HwyMapping):
    '''
    Highway Link Mapping(include main link, IC link)
    '''

    def __init__(self, hwy_object):
        '''
        Constructor
        '''
        HwyMapping.__init__(self)
        self.data_mng = hwy_object.data_mng
        self.G = self.data_mng.get_graph()
        self.hwy = hwy_object

    def _make_main_link_mapping(self):
        '''本线link的Mapping'''
        self.log.info('Start Making Main Link Mapping.')
        self.pg.connect1()
        for road_code, path in self.data_mng.get_road_code_path():
            bwd_ic_nos = []  # 后方(车流方向)
            fwd_ic_nos = []  # 前方(车流方向)
            bwd_ic_no = None
            fwd_ic_no = None
            road_maps = []  # 整条线路的Link
            section_maps = []
            for u, v in zip(path[:-1], path[1:]):
                link_id = self.G.get_linkid(u, v)
                displayclass = self.G.get_disp_class(u, v)
                road_no = self.data_mng.get_road_no(road_code)
                tile_id_14 = self.G.get_tile_id(u, v)
                tile_id = convert_tile_id(tile_id_14)  # 14层tile转成高层tile
                road_kind = UPDOWN_TYPE_UP  # 上行
                if(self.G.is_hwy_node(u) or
                   self.hwy.is_hwy_tile_boundary(u, road_code)):
                    # 取得后方设施
                    if fwd_ic_nos:
                        bwd_ic_nos = fwd_ic_nos
                    else:
                        bwd_ic_nos = self.data_mng.get_ic_nos_by_node(u,
                                                                      road_code
                                                                      )
                    fwd_ic_nos = []
                    fwd_ic_no = None
                # 取得前方设施
                if(self.G.is_hwy_node(v) or
                   self.hwy.is_hwy_tile_boundary(v, road_code)):
                    fwd_ic_nos = self.data_mng.get_ic_nos_by_node(v, road_code)
                if bwd_ic_nos:
                    bwd_ic_no = max(bwd_ic_nos)
                else:
                    self.log.error('Backward ic_no is null. road_code=%s,'
                                   'link_id=%s.' % (road_code, link_id))
                    bwd_ic_no = 0
                if fwd_ic_nos:
                    fwd_ic_no = min(fwd_ic_nos)
                link_map = HwyLinkMapping(road_kind, 1, road_no,
                                          displayclass, link_id,
                                          None, bwd_ic_no,  # fwd_ic先设成空
                                          HWY_PATH_TYPE_MAIN, tile_id)
                section_maps.append(link_map)
                if fwd_ic_no:
                    for link_map in section_maps:
                        link_map.forward_ic_no = fwd_ic_no
                        road_maps.append(link_map)
                    section_maps = []
            if not fwd_ic_no:  # 最后一段线路有没有前方设施
                self.log.error('No Forward IC. road_code=%s' % road_code)
            self._insert_mapping(road_maps)  # 保存整条线路的Link的Mapping情报
        self.pg.commit1()
        self.log.info('End Making Main Link Mapping.')

    def _make_ic_link_mapping(self):
        '''IC(SAPA, JCT, IC) Link的mapping'''
        self.log.info('Start Make IC Link Mapping.')
        # 做成每条IC Link的每个前后设施。
        self._make_ic_link_temp_mapping()
        self.pg.connect1()
        data = self._get_forward_backward_ic()  # 取得link的前后方所有设施
        for fb_info in data:
            link_id = fb_info[0]
            bwd_ic_nos = fb_info[1]
            fwd_ic_nos = fb_info[2]
            bwd_node_ids = fb_info[3]
            fwd_node_ids = fb_info[4]
            displayclass = fb_info[5]
            tile_id_14 = fb_info[6]
            tile_id = convert_tile_id(tile_id_14)
            self.link_type = fb_info[7]
            self.link_id = link_id
            if not bwd_ic_nos and not fwd_ic_nos:
                # self.log.error('1. No Backward/Forward IC for link=%s'
                #                % link_id)
                continue
            bwd_ic_list, bwd_facil_list = self._get_ic_facils(bwd_ic_nos,
                                                              bwd_node_ids)
            fwd_ic_list, fwd_facil_list = self._get_ic_facils(fwd_ic_nos,
                                                              fwd_node_ids)
            road_code, updown = self._get_ic_link_road_code(bwd_facil_list,
                                                            fwd_facil_list)
            if road_code:
                road_no = self.data_mng.get_road_no(road_code)
                road_kind = updown
                # self.data_mng.get_road_kind(road_code, updown)
            else:
                road_no = HWY_INVALID_ROAD_NO
                road_kind = HWY_ROAD_ATTR_NONE
            bwd_ics, fwd_ics = self._get_ic_link_fb_ic(bwd_ic_list,
                                                       fwd_ic_list)
            if not bwd_ics and not fwd_ics:
                self.log.error('2. No Backward/Forward IC for link=%s'
                               % link_id)
                continue
            link_maps = []
            for bwd_ic, fwd_ic in zip(bwd_ics, fwd_ics):
                # 前后设施都为空——通常是无料区间
                if not bwd_ic and not fwd_ic:
                    self.log.error('3. No Backward/Forward IC for link=%s'
                                   % link_id)
                    continue
                link_map = HwyLinkMapping(road_kind, len(bwd_ics), road_no,
                                          displayclass, link_id,
                                          fwd_ic, bwd_ic,
                                          'IC', tile_id)
                link_maps.append(link_map)
            self._insert_mapping(link_maps)
        self.pg.commit1()
        self.log.info('End Make IC Link Mapping.')

    def _make_service_road_mapping(self):
        '''高速辅路'''
        pass

    def _get_ic_facils(self, ic_nos, node_ids):
        '''取得IC，以及设施情报。'''
        rst_facils = []
        rst_ic_no = []
        if not ic_nos:
            return rst_ic_no, rst_facils
        node_ic_dict = {}
        # 同个点的设施归在一起
        for ic_no, node_id in zip(ic_nos, node_ids):
            if node_id and ic_no:
                if node_id in node_ic_dict:
                    node_ic_dict[node_id].append(ic_no)
                else:
                    node_ic_dict[node_id] = [ic_no]
        for node_id, ic_nos in node_ic_dict.iteritems():
            # 同个点上有多几个设施中经过该link，选择一个优先级最高的
            ic_no, facil = self._get_max_priority_facil(node_id,
                                                        ic_nos)
            if ic_no and facil:
                rst_ic_no.append(ic_no)
                rst_facils.append(facil)
            else:
                print 'nodeid=%s, ic_nos=%s' % (node_id, ic_nos)
        return rst_ic_no, rst_facils

    def _get_max_priority_facil(self, node_id, ic_nos):
        '''当前几个设施中(同个点上)，选择一个优先级最高的'''
        # SAPA > JCT > IC; 另外, 同等级时, 父 > 子
        ic_list, facils = self._get_facils(node_id, ic_nos)
        max_facil = facils[0]
        # 取得等级最高的一个设施及ic_no
        for facil in facils[1:]:
            if facil.facilcls in (HWY_IC_TYPE_SA, HWY_IC_TYPE_PA):
                if (max_facil.facilcls not in (HWY_IC_TYPE_SA,
                                               HWY_IC_TYPE_PA)):
                    max_facil = facil
                else:
                    if max_facil.facilcls == facil.facilcls:
                        # 取得二者中的父设施
                        parent = self._get_parent_facil(max_facil, facil)
                        if parent:
                            max_facil = parent
#                         else:
#                             self.log.warning('No Parent. link=%s,node=%s'
#                                              % (self.link_id, node_id))
                    else:
                        self.log.error('Two SAPA: link=%s, node=%s,'
                                       'facilcls=%s, facilcls=%s'
                                       % (self.link_id, node_id,
                                          max_facil.facilcls, facil.facilcls))
            elif facil.facilcls == HWY_IC_TYPE_JCT:
                if (max_facil.facilcls not in (HWY_IC_TYPE_SA, HWY_IC_TYPE_PA,
                                               HWY_IC_TYPE_JCT)):
                    max_facil = facil
                else:
                    if max_facil.facilcls == facil.facilcls:
                        # 取得二者中的父设施
                        parent = self._get_parent_facil(max_facil, facil)
                        if parent:
                            max_facil = parent
#                         else:
#                             self.log.warning('No Parent. link=%s,node=%s'
#                                              % (self.link_id, node_id))
            elif facil.facilcls in (HWY_IC_TYPE_RAMP, HWY_IC_TYPE_IC):
                if (max_facil.facilcls in (HWY_IC_TYPE_SA, HWY_IC_TYPE_PA,
                                           HWY_IC_TYPE_JCT)):
                    continue
                else:
                    if max_facil.facilcls == facil.facilcls:
                        # 取得二者中的父设施
                        parent = self._get_parent_facil(max_facil, facil)
#                         max_ic = ic_list[facils.index(max_facil)]
#                         temp_ic = ic_list[facils.index(facil)]
#                         if (max_facil.inout == INOUT_TYPE_OUT and
#                             max_ic < temp_ic):
#                             print max_ic, temp_ic
#                         if (max_facil.inout == INOUT_TYPE_IN and
#                             max_ic > temp_ic):
#                             print max_ic, temp_ic
                        if parent:
                            max_facil = parent
                        else:
                            self.log.warning('No Parent. link=%s,node=%s'
                                             % (self.link_id, node_id))
                    else:
                        self.log.error('IC Type: link=%s, node=%s,'
                                       'facilcls=%s, facilcls=%s'
                                       % (self.link_id, node_id,
                                          max_facil.facilcls, facil.facilcls))
            else:
                self.log.error('Error facil_class. node=%s, facil_cls=%s'
                               % (node_id, facil.facilcls))
        max_proi_ic = ic_list[facils.index(max_facil)]
        sapa_ic, sapa_facil = self._get_same_sapa_for_jct(max_proi_ic,
                                                          max_facil)
        if sapa_ic and sapa_facil:
            return sapa_ic, sapa_facil
        return max_proi_ic, max_facil

    def _get_facils(self, node_id, ic_nos):
        facils = []
        ic_list = []
        for ic_no in sorted(set(ic_nos)):
            ic_no, facility_id, facil_list = self.data_mng.get_ic(ic_no)
            for facil in facil_list:
                if facil.node_id == node_id:
                    # 按设施各别排序: SA > PA > JCT > Ramp > IC > SmartIC
                    ic_idx = 0
                    while ic_idx < len(facils):
                        if facil.facilcls < facils[ic_idx].facilcls:
                            break
                        elif facil.facilcls == facils[ic_idx].facilcls:
                            # 取得二者中的父设施
                            parent = self._get_parent_facil(facils[ic_idx],
                                                            facil)
                            if parent == facil:
                                break
                        else:
                            pass
                        ic_idx += 1
                    facils.insert(ic_idx, facil)
                    ic_list.insert(ic_idx, ic_no)
        return ic_list, facils

    def _get_same_sapa_for_jct(self, ic_no, facil):
        # 取得JCT设施点上的SAPA
        if facil.facilcls != HWY_IC_TYPE_JCT:
            return None, None
        sapa_ic_no, sapa_facil = self._get_same_sapa(ic_no, facil)
#         if sapa_ic and sapa_facil:
#             if facil.inout == INOUT_TYPE_IN and sapa_ic > ic_no:
#                 print ic_no
#             if facil.inout == INOUT_TYPE_OUT and sapa_ic < ic_no: # 草津ＰＡ
#                 print ic_no
        return sapa_ic_no, sapa_facil

    def _get_same_sapa(self, ic_no, jct_facil):
        '''取得并列，且同点的SAPA。'''
        node_id = jct_facil.node_id
        inout = jct_facil.inout
        # backward
        bwd_ic_no = ic_no - 1
        while bwd_ic_no:
            bwd_ic_no, facil_id, facil_list = self.data_mng.get_ic(bwd_ic_no)
            same_node_flg = False
            for facil in facil_list:
                if facil.node_id == node_id:
                    same_node_flg = True
                    if(facil.facilcls in (HWY_IC_TYPE_SA, HWY_IC_TYPE_PA)
                       and inout == facil.inout):
                        return bwd_ic_no, facil
            if not same_node_flg:  # 非并设、并设结束
                break
            bwd_ic_no -= 1
        # forward
        fwd_ic_no = ic_no + 1
        while True:
            fwd_ic_no, facil_id, facil_list = self.data_mng.get_ic(fwd_ic_no)
            if not fwd_ic_no:
                break
            same_node_flg = False
            for facil in facil_list:
                if facil.node_id == node_id:
                    same_node_flg = True
                    if(facil.facilcls in (HWY_IC_TYPE_SA, HWY_IC_TYPE_PA)
                       and inout == facil.inout):
                        return fwd_ic_no, facil
            if not same_node_flg:  # 非并设、并设结束
                break
            fwd_ic_no += 1
        return None, None

    def _get_parent_facil(self, facil_1, facil_2):
        '''判断二者谁是父设施，并返回。如果都不是，返回None。'''
        return None

    def _get_ic_link_fb_ic(self, bwd_ic_nos, fwd_ic_nos):
        '''取得前后设施'''
        # 前后各只有一个设施    前后刚好做成一条记录
        # 后一条以上，前无。    后有几条，做成几条记录，其中所有前都填0
        # 后一条，前大于一条。    只记录后的一条，前填0
        # 前一条以上，后无。    前有几条，做成几条记不，其中所有后都填0
        # 前一条，后大于一条。    只记录前的一条，后填0
        # 前后都是多条的情况    几个后设施做成多条记录，前都填成0
        bwd_len = 0
        fwd_len = 0
        if bwd_ic_nos:
            bwd_len = len(bwd_ic_nos)
        if fwd_ic_nos:
            fwd_len = len(fwd_ic_nos)
        if bwd_len == 1 and fwd_len == 1:
            return bwd_ic_nos, fwd_ic_nos
        if bwd_len > 0 and fwd_len == 0:
            return bwd_ic_nos, [HWY_INVALID_IC_NO] * bwd_len
        if bwd_len == 1 and fwd_len > 1:
            return bwd_ic_nos, [HWY_INVALID_IC_NO]
        if bwd_len == 0 and fwd_len > 0:
            return [HWY_INVALID_IC_NO] * fwd_len, fwd_ic_nos
        if bwd_len > 1 and fwd_len == 1:
            return [HWY_INVALID_IC_NO] * fwd_len, fwd_ic_nos
        if bwd_len > 1 and fwd_len > 1:
            return bwd_ic_nos, [HWY_INVALID_IC_NO] * bwd_len
        return [], []

    def _get_ic_link_road_code(self, bwd_facls_list, fwd_facls_list):
        # 只有前设施，无后设施 ,且前设施番号，上下行相同==> 前设施道路番号
        # 无前设施，只有后设施,且后设施番号，上下行相同==> 后施道路番号
        # 前后设施所在道路番号相同，且上下行相同==>设施所在道路番号
        # 其他 ==> INVALID_ROAD_NO
        # ## 后设施
        bwd_road_code = None
        bwd_updown = None
        for facil in bwd_facls_list:
            if bwd_road_code and bwd_road_code != facil.road_code:
                return None, HWY_UPDOWN_TYPE_NONE
            if bwd_updown and bwd_updown != facil.updown:
                return None, HWY_UPDOWN_TYPE_NONE
            bwd_road_code = facil.road_code
            bwd_updown = facil.updown
        # ## 前设施
        fwd_road_code = None
        fwd_updown = None
        for facil in fwd_facls_list:
            if fwd_road_code and fwd_road_code != facil.road_code:
                return None, HWY_UPDOWN_TYPE_NONE
            if fwd_updown and fwd_updown != facil.updown:
                return None, HWY_UPDOWN_TYPE_NONE
            fwd_road_code = facil.road_code
            fwd_updown = facil.updown
        if bwd_road_code and not fwd_road_code:
            return bwd_road_code, bwd_updown
        if not bwd_road_code and fwd_road_code:
            return fwd_road_code, fwd_updown
        if bwd_road_code == fwd_road_code and bwd_updown == fwd_updown:
            return bwd_road_code, bwd_updown
        return None, HWY_UPDOWN_TYPE_NONE

    def _check_main_link(self):
        '''判断本线link都被收录。'''
        sqlcmd = '''
        SELECT count(link_id)
          FROM hwy_link_road_code_info
          where link_id not in (
                SELECT link_id
                  FROM highway_mapping
          );
        '''
        self.pg.execute2(sqlcmd)
        row = self.pg.fetchone2()
        if row and row[0] == 0:
            return True
        return False

    def _check_ic_link(self):
        '''判断IC link(Ramp,JCT,SAPA)都被收录。'''
        return True
        sqlcmd = '''
        SELECT count(link_id)
          FROM mid_hwy_ic_link_temp_mapping
          where link_id not in (
                SELECT link_id
                  FROM highway_mapping)
                and toll_flag = 1;
        '''
        self.pg.execute2(sqlcmd)
        row = self.pg.fetchone2()
        if row and row[0] == 0:
            return True
        return False

    def _make_ic_link_temp_mapping(self):
        '''做成每条IC Link的每个前后设施。'''
        self.CreateTable2('mid_temp_hwy_ic_link_mapping')
        sqlcmd = """
        INSERT INTO mid_temp_hwy_ic_link_mapping(
                    bwd_node_id, bwd_ic_no, fwd_node_id,
                    fwd_ic_no, link_id, path_type)
        (
        SELECT distinct bwd_node_id, bwd_ic_no,
               fwd_node_id, fwd_ic_no,
               unnest(regexp_split_to_array(link_lid, E'\\,+')
                     )::bigint as link_id,
               'IC' as path_type
          FROM (
            -- SAPA/JCT OUT
            SELECT a.node_id as bwd_node_id, bwd.ic_no as bwd_ic_no,
                   to_node_id as fwd_node_id, fwd.ic_no as fwd_ic_no,
                   link_lid
              FROM mid_temp_hwy_ic_path as a
              LEFT JOIN mid_hwy_ic_no as bwd  -- Get Backward IC NO
              ON a.road_code = bwd.road_code and
                 a.road_seq = bwd.road_seq and
                 a.facilcls_c = bwd.facilclass_c and
                 a.inout_c = bwd.inout_c and
                 a.node_id = bwd.node_id
              LEFT JOIN mid_hwy_ic_no as fwd  -- Get Forward IC NO
              ON a.facilcls_c = fwd.facilclass_c and
                 fwd.inout_c = 1 and  -- IN
                 a.to_node_id = fwd.node_id
              WHERE a.facilcls_c in (1, 2, 3) and a.inout_c = 2 -- SAPA/JCT OUT
                    and link_lid <> '' and link_lid is not null
            UNION
            ---------------------------------------------------------------
            -- IC/Ramp OUT
            SELECT a.node_id as bwd_node_id, bwd.ic_no as bwd_ic_no,
                   NULL::bigint as fwd_node_id, NULL::bigint as fwd_ic_no,
                   link_lid
              FROM mid_temp_hwy_ic_path as a
              LEFT JOIN mid_hwy_ic_no as bwd  -- Get Backward IC NO
              ON a.road_code = bwd.road_code and
                 a.road_seq = bwd.road_seq and
                 a.facilcls_c = bwd.facilclass_c and
                 a.inout_c = bwd.inout_c and
                 a.node_id = bwd.node_id
              WHERE a.facilcls_c in (5, 7) and a.inout_c = 2  -- IC/Ramp OUT
                    and link_lid <> '' and link_lid is not null
            UNION
            ---------------------------------------------------------------
            -- IC/Ramp IN, (IN's Path is reverse.)
            SELECT NULL::bigint as bwd_node_id, NULL::bigint as bwd_ic_no,
                   a.node_id as fwd_node_id, fwd.ic_no as fwd_ic_no,
                   link_lid
              FROM mid_temp_hwy_ic_path as a
              LEFT JOIN mid_hwy_ic_no as fwd  -- Get Backward IC NO
              ON a.road_code = fwd.road_code and
                 a.road_seq = fwd.road_seq and
                 a.facilcls_c = fwd.facilclass_c and
                 a.inout_c = fwd.inout_c and
                 a.node_id = fwd.node_id
              WHERE a.facilcls_c in (5, 7) and a.inout_c = 1  -- IC/Ramp IN
                    and link_lid <> '' and link_lid is not null
          ) as c
          ORDER BY link_id
        );
        """
        self.pg.execute2(sqlcmd)
        self.pg.commit2()

    def _get_forward_backward_ic(self):
        '''取得link的所有设施'''
        sqlcmd = """
        SELECT a.link_id,
               bwd_ic_nos,
               fwd_ic_nos,
               bwd_node_ids,
               fwd_node_ids,
               display_class,
               tile_id,
               link_type
          FROM (
            SELECT link_id,
                   array_agg(bwd_ic_no) as bwd_ic_nos,
                   array_agg(fwd_ic_no) as fwd_ic_nos,
                   array_agg(bwd_node_id) as bwd_node_ids,
                   array_agg(fwd_node_id) as fwd_node_ids
              FROM mid_temp_hwy_ic_link_mapping
              group by link_id
          ) AS a
          LEFT JOIN link_tbl
          ON a.link_id = link_tbl.link_id
          order by link_id;
        """
        return self.pg.get_batch_data2(sqlcmd)
