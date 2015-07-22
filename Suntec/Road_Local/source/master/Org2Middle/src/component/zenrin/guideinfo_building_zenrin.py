# -*- coding: UTF-8 -*-
'''
Created on 2015-5-4

@author: wushengbing
'''
import common.cache_file
import component.default.multi_lang_name
import component.default.guideinfo_building




class comp_guideinfo_building_zenrin(component.default.guideinfo_building.comp_guideinfo_building):
    def __init__(self):
        '''
        Constructor
        '''
        component.default.guideinfo_building.comp_guideinfo_building.__init__(self)
      

    def _Do(self):
        self._loadBrandIcon()
        self._loadPOICategory()
        self._loadCategoryPriority()
        self._findLogmark()
        self._makePOIName()
        self._makeLogmark()
        return 0

                   
           
    def _findLogmark(self):       

        self.log.info('make temp_poi_logmark...')
        self.CreateTable2('temp_poi_logmark')
        
        sqlcmd = """
            insert into temp_poi_logmark
            (
                select a.poi_id, a.sub_cat,tpc.u_code,a.the_geom_4326 as the_geom
                from org_poi as a
                left join temp_poi_category as tpc
                on tpc.org_code::bigint = a.sub_cat
                inner join temp_brand_icon as b
                on a.sub_cat = b.brandname::bigint
               
               union
               
                select a.poi_id, a.sub_cat,tpc.u_code,a.the_geom_4326 as the_geom
                from org_poi as a
                left join temp_poi_category as tpc
                on tpc.org_code::bigint = a.sub_cat
                where a.sub_cat = 315200 and a.series in ('好樂迪ＫＴＶ','錢櫃ＰＡＲＴＹ　ＷＯＲＬＤ')
               
               union
               
                select a.poi_id, a.sub_cat,tpc.u_code,a.the_geom_4326 as the_geom
                from org_poi as a
                left join temp_poi_category as tpc
                on tpc.org_code::bigint = a.sub_cat
                inner join temp_category_priority as c
                on c.u_code = tpc.u_code and c.category_priority in (1,2,4)
                
            );                                 
                """

        self.pg.execute(sqlcmd)
        self.pg.commit2()
        self.CreateIndex2('temp_poi_logmark_poi_id_idx')
   
   
    def _makePOIName(self):
        self.log.info('make temp_poi_name...')
        self.CreateTable2('temp_poi_name')
        
        if not component.default.multi_lang_name.MultiLangName.is_initialized():
            component.default.multi_lang_name.MultiLangName.initialize()
  
        sqlcmd = """
                select  poi_id,
                    array_agg(name_id) as name_id_array,
                    array_agg(name_type) as name_type_array,
                    array_agg(language) as language_code_array,
                    array_agg(poi_name) as name_array,
                    array_agg(phoneme_lang) as phonetic_language_array,
                    array_agg(phoneme) as phoneme_array
                from 
                    (
                        select  a.poi_id,
                                a.offi_name as poi_name,
                                'CHT'::varchar as language,
                                1 as name_id,
                                'office_name'::varchar as name_type,
                                'PYM'::varchar as phoneme_lang,
                                a.offi_name_py_pym as phoneme,
                                a.the_geom
                        from org_poi as a   
                        order by poi_id                                               
                    ) as b
                group by poi_id
                order by poi_id;
                """                      
        asso_recs = self.pg.get_batch_data2(sqlcmd)
        
        temp_file_obj = common.cache_file.open('poi_name')
        for asso_rec in asso_recs:
            poi_id = asso_rec[0]
            json_name = component.default.multi_lang_name.MultiLangName.name_array_2_json_string_multi_phon(asso_rec[1], 
                                                                                                            asso_rec[2], 
                                                                                                            asso_rec[3], 
                                                                                                            asso_rec[4],
                                                                                                            asso_rec[5], 
                                                                                                            asso_rec[6])
            temp_file_obj.write('%d\t%s\n' % (int(poi_id), json_name))
        
        temp_file_obj.seek(0)
        self.pg.copy_from2(temp_file_obj, 'temp_poi_name')
        self.pg.commit2()
        common.cache_file.close(temp_file_obj,True)
        self.CreateIndex2('temp_poi_name_poi_id_idx')         

    
    def _makeLogmark(self):
        self.log.info('make mid_logmark...')
        self.CreateTable2('mid_logmark')
        
        sqlcmd = """
                insert into mid_logmark(poi_id, type_code,type_code_priority, building_name, the_geom)
                select  a.poi_id, 
                        a.u_code,
                        c.category_priority,
                        b.poi_name,
                        a.the_geom
                from temp_poi_logmark as a
                left join temp_poi_name as b
                on a.poi_id = b.poi_id
                left join temp_category_priority as c
                on c.u_code = a.u_code
                order by a.poi_id;      
                """
        self.pg.execute(sqlcmd)
        self.pg.commit2()
        
        self.CreateIndex2('mid_logmark_the_geom_idx')
        

        