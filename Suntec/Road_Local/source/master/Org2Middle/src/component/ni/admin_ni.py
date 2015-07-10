# -*- coding: UTF8 -*-
'''
Created on 2015-5-6

@author: zhaojie
'''

import component.component_base
import component.default.multi_lang_name

class comp_admin_ni(component.component_base.comp_base):
    '''
    classdocs
    '''


    def __init__(self):
        '''
        Constructor 
        '''
        component.component_base.comp_base.__init__(self, 'Admin')
        
    def _DoCreateTable(self):
        self.CreateTable2('mid_admin_zone')
    
    def _Do(self):
        
        self.__get_order8()
        self.__get_order2()
        self.__get_order1()
        self.__get_order0()
        self.__insert_into_admin()
        
        return 0
    
    def __get_order8(self):
        self.log.info('Begin get order_08')
        
        self.CreateTable2('temp_admin_order8')
        
        self.CreateIndex2('org_d_admincode_idx')
        self.CreateIndex2('org_admin_admincode_idx')
        
        sqlcmd = '''
                insert into temp_admin_order8(order8_id, order2_id, country_code, the_geom)
                (
                    select a.admincode, cityadcode, 'CHN', st_multi(st_union(the_geom)) as the_geoms
                    from org_admin as a
                    left join org_d as b
                    on a.admincode = b.admincode and b.kind = '0137'
                    group by a.admincode, cityadcode
                    order by a.admincode, cityadcode               
                );
                '''       
        self.pg.execute2(sqlcmd)
        self.pg.commit2()
        
        self.CreateIndex2('temp_admin_order8_order8_id_idx')
        return 0
    
    def __get_order2(self):
        self.log.info('Begin get order_02')
        
        self.CreateTable2('temp_admin_order2')

        self.CreateIndex2('org_admin_cityadcode_idx')
        
        sqlcmd = '''
                insert into temp_admin_order2(order2_id, order1_id, country_code, the_geom)
                (
                    select cityadcode, proadcode, 'CHN', st_multi(st_union(the_geom)) as the_geoms
                    from org_admin as a
                    left join org_d as b
                    on a.admincode = b.admincode and b.kind = '0137'
                    group by cityadcode, proadcode
                    order by cityadcode, proadcode
                );
                '''
        
        self.pg.execute2(sqlcmd)
        self.pg.commit2()
        
        self.CreateIndex2('temp_admin_order2_order2_id_idx')
        
        return 0
    
    def __get_order1(self):
        self.log.info('Begin get order_01')
        
        self.CreateTable2('temp_admin_order1')

        self.CreateIndex2('org_admin_proadcode_idx')
        
        sqlcmd = '''
                insert into temp_admin_order1(order1_id, order0_id, country_code, the_geom)
                (
                    select proadcode, '100000', 'CHN', st_multi(st_union(the_geom)) as the_geoms
                    from org_admin as a
                    left join org_d as b
                    on a.admincode = b.admincode and b.kind = '0137'
                    group by proadcode
                    order by proadcode
                );        
                '''
        
        self.pg.execute2(sqlcmd)
        self.pg.commit2()
        
        self.CreateIndex2('temp_admin_order1_order1_id_idx')
        
        return 0
    
    def __get_order0(self):
        self.log.info('Begin get order_00')
        
        self.CreateTable2('temp_admin_order0')
        
        sqlcmd = '''
                insert into temp_admin_order0(order0_id, order0_code, country_code, the_geom)
                (
                    select '100000', '100000', 'CHN', st_multi(st_union(the_geom)) as the_geoms
                    from org_admin as a
                    left join org_d as b
                    on a.admincode = b.admincode and b.kind = '0137'
                    group by kind
                    order by kind                    
                );
                '''       
        self.pg.execute2(sqlcmd)
        self.pg.commit2()
        
        return 0
    
    def __insert_into_admin(self):
        self.log.info('Begin get mid_admin_zone')
        
        sqlcmd = '''
                insert into mid_admin_zone(ad_code, ad_order, order0_id, order1_id, 
                    order2_id, order8_id, ad_name, time_zone, summer_time_id, the_geom)
                (
                    select order0_id::integer, 0, order0_id::integer, null::integer, null::integer,
                        null::integer, b.admin_name,200, -1, the_geom
                    from temp_admin_order0 as a
                    left join temp_admin_name as b
                    on a.order0_id = b.admin_id
                    
                    union
                    
                    select order1_id::integer, 1, order0_id::integer, order1_id::integer, 
                        null::integer, null::integer, b.admin_name, 200, -1, the_geom
                    from temp_admin_order1 as a
                    left join temp_admin_name as b
                    on a.order1_id = b.admin_id
                    
                    union
                    
                    select order2_id::integer, 2, order0_id::integer, a.order1_id::integer, 
                        order2_id::integer, null::integer, b.admin_name,200, -1, a.the_geom
                    from temp_admin_order2 as a
                    left join temp_admin_name as b
                    on a.order2_id = b.admin_id
                    left join temp_admin_order1 as c
                    on a.order1_id = c.order1_id
                    
                    union
                    
                    select order8_id::integer, 8, d.order0_id::integer, c.order1_id::integer, 
                        c.order2_id::integer, order8_id::integer, b.admin_name, 200, -1, a.the_geom
                    from temp_admin_order8 as a
                    left join temp_admin_name as b
                    on a.order8_id = b.admin_id
                    left join temp_admin_order2 as c
                    on a.order2_id = c.order2_id
                    left join temp_admin_order1 as d
                    on c.order1_id = d.order1_id
                );
                '''
        
        self.pg.execute2(sqlcmd)
        self.pg.commit2()
        
        self.CreateIndex2('mid_admin_zone_ad_code_idx')
        self.CreateIndex2('mid_admin_zone_ad_order_idx')
        self.CreateIndex2('mid_admin_zone_order0_id_idx')
        self.CreateIndex2('mid_admin_zone_order1_id_idx')
        self.CreateIndex2('mid_admin_zone_order2_id_idx')
        self.CreateIndex2('mid_admin_zone_order8_id_idx')
        self.CreateIndex2('mid_admin_zone_the_geom_idx')
 
        return 0
     
        
        