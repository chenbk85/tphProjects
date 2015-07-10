# -*- coding: cp936 -*-
'''
Created on 2015-5-5

@author:
'''



import platform.TestCase
import time

class CCheckOrg_Cond_TableStruct(platform.TestCase.CTestCase):
    def _do(self):
        return self.pg.IsExistTable('org_cond') and self.pg.IsExistTable('org_cnl') and self.pg.IsExistTable('org_cr')

class CCheckOrg_Cond_condtype(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(gid)
                    from org_cond 
                    where 
                        condtype::bigint = 2
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec <> 0)

class CCheckOrg_Cond_mapid(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(gid)
                    from org_cond 
                    where 
                        condtype::bigint = 2 and mapid = ''
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cond_condid(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(gid)
                    from org_cond 
                    where 
                        condtype::bigint = 2 and condid = ''
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cond_crid(platform.TestCase.CTestCase):
    def _do(self):
        self.pg.CreateIndex_ByName('org_cond_crid_idx')
        self.pg.CreateIndex_ByName('org_cr_crid_idx')
        sqlcmd = """
                    select count(b.crid)
                    from org_cond a
                    left join org_cr b
                        on a.crid = b.crid
                    where 
                        condtype::bigint = 2 and 
                        a.crid != ''
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cond_passage(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(gid)
                    from org_cond 
                    where 
                        condtype::bigint = 2 and 
                        passage != '' 
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cond_slope(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(gid)
                    from org_cond 
                    where 
                        condtype::bigint = 2 and 
                        slope != '' 
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cond_sgnl_loction(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(gid)
                    from org_cond 
                    where 
                        condtype::bigint = 2 and 
                        sgnl_loction != '' 
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cond_mapid_condid(platform.TestCase.CTestCase):
    def _do(self):
        self.pg.CreateIndex_ByName('org_cond_mapid_condid_idx')
        self.pg.CreateIndex_ByName('org_cnl_mapid_condid_idx')
        sqlcmd = """
                    select count(b.gid)
                    from org_cond a
                    left join org_cnl b
                        on 
                            a.mapid::bigint = b.mapid::bigint and
                            a.condid::bigint = b.condid::bigint
                    where 
                        condtype::bigint = 2 and 
                        b.gid IS NULL
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cnl_mapid_condid(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(b.gid)
                    from org_cond a
                    left join org_cnl b
                        on 
                            a.mapid::bigint = b.mapid::bigint and
                            a.condid::bigint = b.condid::bigint
                    where 
                        condtype::bigint = 2 and 
                        (b.mapid = '' or
                        b.condid = '')
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cnl_mapid_linkid(platform.TestCase.CTestCase):
    def _do(self):
        self.pg.CreateIndex_ByName('org_cnl_mapid_linkid_idx')
        self.pg.CreateIndex_ByName('org_r_mapid_id_idx')
        sqlcmd = """
                    select count(*)
                    from (
                        select c.*
                        from org_cond a
                        left join org_cnl b
                            on 
                                a.mapid::bigint = b.mapid::bigint and
                                a.condid::bigint = b.condid::bigint
                        left join org_r c
                            on
                                b.mapid::bigint = c.mapid::bigint and
                                b.linkid::bigint = c.id::bigint
                        where 
                            a.condtype::bigint = 2 and
                            b.gid IS NOT NULL and
                            b.linkid != ''
                    ) as d
                    where gid IS NULL
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cnl_mapid_nodeid(platform.TestCase.CTestCase):
    def _do(self):
        self.pg.CreateIndex_ByName('org_cnl_mapid_nodeid_idx')
        self.pg.CreateIndex_ByName('org_n_mapid_id_idx')
        sqlcmd = """
                    select count(*)
                    from (
                        select c.*
                        from org_cond a
                        left join org_cnl b
                            on 
                                a.mapid::bigint = b.mapid::bigint and
                                a.condid::bigint = b.condid::bigint
                        left join org_n c
                            on
                                b.mapid::bigint = c.mapid::bigint and
                                b.nodeid::bigint = c.id::bigint
                        where 
                            a.condtype::bigint = 2 and
                            b.gid IS NOT NULL and
                            b.nodeid != ''
                    ) as d
                    where gid IS NULL
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cnl_seq_nm(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(*)
                    from (
                        select b.*
                        from org_cond a
                        left join org_cnl b
                            on 
                                a.mapid::bigint = b.mapid::bigint and
                                a.condid::bigint = b.condid::bigint
                        where 
                            a.condtype::bigint = 2 and
                            b.gid IS NOT NULL
                    ) as c
                    where seq_nm = ''
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cnl_angle(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(*)
                    from (
                        select b.*
                        from org_cond a
                        left join org_cnl b
                            on 
                                a.mapid::bigint = b.mapid::bigint and
                                a.condid::bigint = b.condid::bigint
                        where 
                            a.condtype::bigint = 2 and
                            b.gid IS NOT NULL
                    ) as c
                    where 
                        (seq_nm::bigint in (1, 2) and angle != '') or
                        (seq_nm::bigint not in (1, 2) and angle = '')
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckOrg_Cnl_linkid_seq_nm(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(*)
                    from (
                        select b.*
                        from org_cond a
                        left join org_cnl b
                            on 
                                a.mapid::bigint = b.mapid::bigint and
                                a.condid::bigint = b.condid::bigint
                        where 
                            a.condtype::bigint = 2 and
                            b.gid IS NOT NULL
                    ) as c
                    where 
                        (seq_nm::bigint != 2 and linkid = '') or
                        (seq_nm::bigint = 2 and linkid != '')
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)
    
class CCheckOrg_Cnl_nodeid_seq_nm(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(*)
                    from (
                        select b.*
                        from org_cond a
                        left join org_cnl b
                            on 
                                a.mapid::bigint = b.mapid::bigint and
                                a.condid::bigint = b.condid::bigint
                        where 
                            a.condtype::bigint = 2 and
                            b.gid IS NOT NULL
                    ) as c
                    where
                        seq_nm::bigint = 2 and nodeid = ''
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)
    
class CCheckOrg_Cnl_linkid_nodeid(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(*)
                    from (
                        select a.condid, c.* 
                        from org_cond a
                        left join org_cnl b
                            on 
                                a.mapid::bigint = b.mapid::bigint and
                                a.condid::bigint = b.condid::bigint
                        left join org_r c
                            on
                                b.mapid::bigint = c.mapid::bigint and
                                b.linkid::bigint = c.id::bigint
                        where 
                            a.condtype::bigint = 2 and
                            b.gid IS NOT NULL and 
                            b.seq_nm::bigint = 1
                    ) as d
                    left join (
                        select e.condid, g.*
                        from org_cond e
                        left join org_cnl f
                            on
                                e.mapid::bigint = f.mapid::bigint and
                                e.condid::bigint = f.condid::bigint
                        left join org_n g
                            on 
                                f.mapid::bigint = g.mapid::bigint and
                                f.nodeid::bigint = g.id::bigint
                        where 
                            e.condtype::bigint = 2 and
                            f.gid IS NOT NULL and
                            f.seq_nm::bigint = 2 
                    ) as h
                        on 
                            d.mapid::bigint = h.mapid::bigint and
                            d.condid::bigint = h.condid::bigint
                    where
                        h.id not in (d.snodeid, d.enodeid)
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)
    
class CCheckForce_Guide_Tbl_TableStruct(platform.TestCase.CTestCase):
    def _do(self):
        return self.pg.IsExistTable('force_guide_tbl')

class CCheckForce_Guide_Tbl_Force_Guide_Id(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(gid)
                    from force_guide_tbl
                    where force_guide_id is null
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)
    
class CCheckForce_Guide_Tbl_Nodeid(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(a.nodeid)
                    from force_guide_tbl as a
                    left join node_tbl as b
                        on a.nodeid = b.node_id
                    where b.node_id is null
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckForce_Guide_Tbl_Inlinkid(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(a.inlinkid)
                    from force_guide_tbl as a
                    left join link_tbl as b
                        on a.inlinkid = b.link_id
                    where b.link_id is null
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckForce_Guide_Tbl_Inlinkid_Nodeid_Rel(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                select count(*)
                from force_guide_tbl as a
                left join link_tbl as b
                    on a.inlinkid = b.link_id
                where a.nodeid not in (b.s_node, b.e_node)
                """
        nRecCount = self.pg.getOnlyQueryResult(sqlcmd)
        return (nRecCount == 0)

class CCheckForce_Guide_Tbl_Outlinkid(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                    select count(a.outlinkid)
                    from force_guide_tbl as a
                    left join link_tbl as b
                        on a.outlinkid = b.link_id
                    where b.link_id is null
                 """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckForce_Guide_Tbl_PassLink_Cnt(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                select count(*)
                from
                (
                    select (case 
                            when string_to_array(passlid, '|') is null then 0
                            else array_upper(string_to_array(passlid, '|'),1)
                            end
                           ) as lenth, passlink_cnt
                    FROM force_guide_tbl
                )as a
                where a.lenth <> passlink_cnt
                """
        rec_cnt = self.pg.getOnlyQueryResult(sqlcmd)
        return (rec_cnt == 0)
    
class CCheckForce_Guide_Tbl_Guide_Type(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                select count(force_guide_id)
                from force_guide_tbl
                where 
                    not (guide_type >= 0 and guide_type <= 13) and 
                    not (guide_type = 255) and 
                    not (guide_type >= 32 and guide_type <= 43) and
                    not (guide_type >= 48 and guide_type <= 59) and 
                    not (guide_type >= 64 and guide_type <= 75) and 
                    not (guide_type >= 80 and guide_type <= 248)
                """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckForce_Guide_Tbl_Position_Type(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                select count(force_guide_id)
                from force_guide_tbl
                where not (position_type >= 0 and position_type <= 9)
                """
        count_rec = self.pg.getOnlyQueryResult(sqlcmd)
        return (count_rec == 0)

class CCheckForce_Guide_Tbl_Inlinkid_Outlinkid_Equal(platform.TestCase.CTestCase):
    def _do(self):
        sqlcmd = """
                select count(inlinkid)
                from force_guide_tbl
                where inlinkid = outlinkid
                """
        nRecCount = self.pg.getOnlyQueryResult(sqlcmd)
        return (nRecCount == 0)

class CCheckForce_Guide_Tbl_Inlink_Outlink_Conn(platform.TestCase.CTestCase):
    '''check guide ������node_id����Ч�ԣ�inlink��outlink��ͨ��'''
    
    def _do(self): 
        try:
            if self.pg.CreateFunction_ByName('mid_check_force_guide_links') == -1:
                time.sleep(60)
        except:
            self.pg.rollback()
            pass
                    
        sqlcmd = """
            select count(*) 
            from (
                select 
                    inlinkid,
                    one_way_code,
                    nodeid,outlinkid,
                    passlink_cnt,
                    dir1,
                    dir2,
                    mid_check_force_guide_links(inlinkid,nodeid,passlink_cnt,outlinkid,dir1::smallint) as pass_flag1,
                    case 
                        when dir2 is not null then mid_check_force_guide_links(inlinkid,nodeid,passlink_cnt,outlinkid,dir2::smallint) 
                    end as pass_flag2
                from (
                    select  
                        inlinkid,
                        b.one_way_code, 
                        nodeid,
                        outlinkid,
                        passlink_cnt,
                        case
                            when b.one_way_code = 1 and a.nodeid = b.s_node then 0
                            when b.one_way_code = 1 and a.nodeid = b.e_node then 1
                            when b.one_way_code  = 2 and a.nodeid = b.s_node then 0
                            when b.one_way_code  = 2 and a.nodeid = b.e_node then 1
                            when b.one_way_code  = 3 and a.nodeid = b.s_node then 1
                            when b.one_way_code  = 3 and a.nodeid = b.e_node then 0
                            else 0
                        end as dir1,
                        case 
                            when b.one_way_code = 1 and a.nodeid = b.s_node then 1
                            when b.one_way_code = 1 and a.nodeid = b.e_node then 0
                            else null
                        end as dir2
                     from force_guide_tbl a
                     left join link_tbl b
                         on a.inlinkid = b.link_id
                     where b.link_id is not null
                ) c
            ) d 
            where pass_flag1 is false and pass_flag2 is false
        """
        # �滻����
        sqlcmd = sqlcmd.replace('[replace_table]', self.caseinfo.getTableName())
        
        self.pg.execute(sqlcmd)
        row = self.pg.fetchone()
        if row:
            if row[0] != 0:
                return False
            else:
                return True
        else:
            return False