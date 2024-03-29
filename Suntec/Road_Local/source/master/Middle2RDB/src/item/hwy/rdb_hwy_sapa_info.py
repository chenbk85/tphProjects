# -*- coding: UTF8 -*-
'''
Created on 2013-10-10

@author: hongchenzai
'''
from base.rdb_ItemBase import ItemBase
HWY_OASIS = 'hwy oasis'
DOG_RUN = 'dog run'
PUBLIC_TEL = 'Public telephone'
VENDING_MACHINE = 'Vending machine'
HANDICAPPED_TEL = 'Handicapped telephone'
HANDICAPPED_TOILET = 'Handicapped toilet'
INFORMATION = 'Info'
SNACK_CORNER = 'Snack corner'
NURSING_ROOM = 'Nursing room'
LAUNDERETTE = 'Launderette'
COIN_SHOWER = 'Coin shower'
TOILET = 'Toilet'
REST_AREA = 'Rest area'
SHOPPING_CORNER = 'Shopping corner'
FAX_SERVICE = 'Fax service'
POST_BOX = 'Post box'
HWY_INFOR_TERMINAL = 'Hwy infor terminal'
ATM = 'ATM'
HWY_HOT_SPRINGS = 'Hwy hot springs'
NAP_REST_AREA = 'Nap rest area'
RESTAURANT = 'Restaurant'
GAS_STATION = 'Gas station'
FIELD_NAME_DICT = {DOG_RUN: ('dog_run', 1),
                   HWY_OASIS: ('hwy_oasis', 2),
                   PUBLIC_TEL: ('public_telephone', 3),
                   VENDING_MACHINE: ('vending_machine', 4),
                   HANDICAPPED_TEL: ('handicapped_telephone', 5),
                   HANDICAPPED_TOILET: ('handicapped_toilet', 6),
                   INFORMATION: ('information', 7),
                   SNACK_CORNER: ('snack_corner', 8),
                   NURSING_ROOM: ('nursing_room', 9),
                   LAUNDERETTE: ('launderette', 10),
                   COIN_SHOWER: ('coin_shower', 11),
                   TOILET: ('toilet', 12),
                   REST_AREA: ('rest_area', 13),
                   SHOPPING_CORNER: ('shopping_corner', 14),
                   FAX_SERVICE: ('fax_service', 15),
                   POST_BOX: ('postbox', 16),
                   HWY_INFOR_TERMINAL: ('hwy_infor_terminal', 17),
                   ATM: ('atm', 18),
                   HWY_HOT_SPRINGS: ('hwy_hot_springs', 19),
                   NAP_REST_AREA: ('nap_rest_area', 20),
                   RESTAURANT: ('restaurant', 21),
                   GAS_STATION: ('gas_station', 22)
                   }


class rdb_highway_sapa_info(ItemBase):
    '''SAPA信息。包括：插图，服务信息
    '''

    def __init__(self):
        '''
        Constructor
        '''
        ItemBase.__init__(self, 'Highway_SAPA')

    def Do(self):
        # 插图
        self._MakeIllust()
        # SAPA服务标志位
        self._MakeServiceInfo()
        self._create_service_info_view()
        self._MakeServiceCategory()
        return 0

    def _MakeIllust(self):
        'SAPA插图'
        self.CreateTable2('rdb_highway_illust_info')
        if self.pg.IsExistTable('highway_illust_info') != True:
            return 0
        sqlcmd = """
        INSERT INTO rdb_highway_illust_info(ic_no, image_id, tile_id)
        (
        select a.ic_no, b.gid as image_id, tile_id
          from highway_illust_info as a
          LEFT JOIN rdb_guideinfo_pic_blob_bytea as b
          on lower(a.image_id) = lower(b.image_id)
          LEFT JOIN rdb_highway_ic_info AS ic
          on a.ic_no = ic.ic_no
          order by ic_no, a.gid
        );
        """
        self.pg.execute2(sqlcmd)
        self.pg.commit2()
        return 0

    def _MakeServiceInfo(self):
        'SAPA服务标志位。'
        self.CreateTable2('rdb_highway_service_info')
        if self.pg.IsExistTable('highway_service_info') != True:
            return 0
        sqlcmd = """
        INSERT INTO rdb_highway_service_info(
                    ic_no, dog_run, hwy_oasis,
                    public_telephone, vending_machine, handicapped_telephone,
                    handicapped_toilet, information, snack_corner,
                    nursing_room, gas_station, launderette,
                    coin_shower, toilet, rest_area,
                    shopping_corner, fax_service, postbox,
                    hwy_infor_terminal, atm, hwy_hot_springs,
                    nap_rest_area, restaurant, tile_id)
        (
        SELECT a.ic_no, dog_run, hwy_oasis,
               public_telephone, vending_machine, handicapped_telephone,
               handicapped_toilet, information, snack_corner,
               nursing_room, gas_station, launderette,
               coin_shower, toilet, rest_area,
               shopping_corner, fax_service, postbox,
               hwy_infor_terminal, atm, hwy_hot_springs,
               nap_rest_area, restaurant, tile_id
          FROM highway_service_info AS a
          LEFT JOIN highway_ic_info AS b
          ON a.ic_no = b.ic_no
          order by ic_no, a.gid
        );
        """
        self.pg.execute2(sqlcmd)
        self.pg.commit2()
        self.CreateIndex2('rdb_highway_service_info_ic_no_idx')
        return 0

    def _create_service_info_view(self):
        sqlcmd = '''
        CREATE VIEW rdb_highway_service_info_view
        AS(
           select ic_no, 'gas_station' as service_typ
           from rdb_highway_service_info
           where gas_station = 1

           union

           select ic_no, 'restaurant' as service_typ
           from rdb_highway_service_info
           where restaurant = 1

           union

           select ic_no, 'shopping_corner' as service_typ
           from rdb_highway_service_info
           where shopping_corner = 1

           union

           select ic_no, 'atm' as service_typ
           from rdb_highway_service_info
           where atm = 1

           union

           select ic_no, 'postbox' as service_typ
           from rdb_highway_service_info
           where postbox = 1

           union

           select ic_no, 'information' as service_typ
           from rdb_highway_service_info
           where information = 1

           union

           select ic_no, 'toilet' as service_typ
           from rdb_highway_service_info
           where toilet = 1

           union

           select ic_no, 'rest area' as service_typ
           from rdb_highway_service_info
           where rest_area = 1
        )
        '''
        self.pg.execute2(sqlcmd)
        self.pg.commit2()

    def _insert_into_service_cate_mapping(self, pram):
        sqlcmd = '''
        INSERT INTO rdb_highway_service_category_mapping(field_name,
                                                         service,
                                                         service_id,
                                                         category_id
                                                         )
        VALUES(%s, %s, %s, %s)
        '''
        self.pg.execute2(sqlcmd, pram)

    def _MakeServiceCategory(self):
        'service category mapping'
        self.CreateTable2('rdb_highway_service_category_mapping')
        if self.pg.IsExistTable('hwy_service_category_mapping') != True:
            return 0
        sqlcmd = '''
            select service, category_id, add_cat_id
            from hwy_service_category_mapping
            where category_id is not null or
                  add_cat_id is not null
        '''
        for row in self.pg.get_batch_data2(sqlcmd):
            service, category_id, add_cat_id = row
            if not category_id:
                category_id = add_cat_id
            field_name, service_id = FIELD_NAME_DICT.get(service)
            if not field_name:
                self.log.error('no field name, service= %s'
                                % service)
                continue
            self._insert_into_service_cate_mapping((field_name,
                                                    service,
                                                    service_id,
                                                    category_id
                                                  ))
        self.pg.commit2()
        return 0
