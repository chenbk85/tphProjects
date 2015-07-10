# -*- coding: UTF-8 -*-
'''
Created on 2012-3-23

@author: sunyifeng
'''

import io
import os

import component.component_base
import common.common_func

class comp_import_sidefiles_jpn(component.component_base.comp_base):

    def __init__(self):

        component.component_base.comp_base.__init__(self, 'sidefiles')
        
    def _DoCreateTable(self):
        if self.CreateTable2('road_highway') == -1:
            return -1
        
        if self.CreateTable2('road_hwysame') == -1:
            return -1
        
        if self.CreateTable2('road_tollguide') == -1:
            return -1
 
        if self.CreateTable2('road_ferry_code') == -1:
            return -1

        if self.CreateTable2('road_gs') == -1:
            return -1

        if self.CreateTable2('road_multiple_store') == -1:
            return -1

        if self.CreateTable2('road_road_code') == -1:
            return -1

        if self.CreateTable2('road_road_code_highway') == -1:
            return -1
        
        if self.CreateTable2('road_roadpoint') == -1:
            return -1
        
        if self.CreateTable2('road_roadpoint_normal') == -1:
            return -1
        
        if self.CreateTable2('road_roadpoint_bunnki') == -1:
            return -1
        
        if self.CreateTable2('road_store_code') == -1:
            return -1
                                                                       
        return 0
    
    def _DoCreateIndex(self):
        
        return 0
    
    def _DoCreateFunction(self):
        
        if self.CreateFunction2('deal_type_of_sidefile') == -1:
            return -1

        if self.CreateFunction2('create_geom_of_sidefile') == -1:
            return -1

                        
        return 0
    
    def _Do(self):
        
        sqlcmd = """
                UPDATE spatial_ref_sys
                  SET proj4text='+proj=longlat +ellps=bessel towgs84=-146.43,507.89,681.46'
                  WHERE srid=4301;
                """
        self.pg.execute2(sqlcmd)
        self.pg.commit2()
        
        kensaku_path = common.common_func.GetPath('sf_highway')
        highway_path = common.common_func.GetPath('highway')
        connectway_path = common.common_func.GetPath('connectway')
                
        self._Do_highway(highway_path)
        self._Do_hwysame(highway_path)
        self._Do_tollguide(highway_path)
        
        self._Do_ferry_code(kensaku_path)        
        self._Do_gs(kensaku_path)
        self._Do_multiple_store(kensaku_path)
        self._Do_road_code(kensaku_path)
        self._Do_road_code_highway(kensaku_path)        
        self._Do_roadpoint(kensaku_path)
        self._Do_roadpoint_normal(kensaku_path)
        self._Do_roadpoint_bunnki(kensaku_path)
        self._Do_store_code(kensaku_path)
        
        self._Do_connectway(connectway_path)     
                                
    def _Do_highway(self,sf_path): 
           
        pg = self.pg
        pgcur = self.pg.pgcur2
        
        f_highway = os.path.join(sf_path, 'highway.txt')
    
        objFileAnalyst = CFileAnalyst_highway(f_highway)
        listrecord = objFileAnalyst.analyse()

        for record in listrecord:
            self.pg.insert('road_highway', 
                          (
                           'road_code',
                           'linkcls_c',
                           'dir_f',
                           'dir_point',
                           'road_point',
                           'next_point',
                           'next_dis',
                           'end_f',
                           'forward_num',
                           'backward_num',
                           'branches',
                          ),
                          (
                           record[0],
                           record[1],
                           record[2],
                           record[3],
                           record[4],
                           record[5],
                           record[6],
                           record[7],
                           record[8],
                           record[9],
                           record[10]
                          )
                          )
        
        self.pg.commit2()
        
        return 0
        
    def _Do_hwysame(self,sf_path):         
            
        pg = self.pg
        pgcur = self.pg.pgcur2
               
        f_hwysame = os.path.join(sf_path, 'hwysame.txt')
    
        objFileAnalyst = CFileAnalyst_hwysame(f_hwysame)
        listrecord = objFileAnalyst.analyse()

        for record in listrecord:
            self.pg.insert('road_hwysame', 
                          (
                           'road_code',
                           'road_point',
                           'dir_f',
                           'name',
                           'roads',
                          ),
                          (
                           record[0],
                           record[1],
                           record[2],
                           record[3],
                           record[4]
                          )
                          )
        
        self.pg.commit2()
        
        return 0
    
    def _Do_tollguide(self,sf_path):
        pg = self.pg
        pgcur = self.pg.pgcur2
               
        f_tollguide = os.path.join(sf_path, 'tollguide.txt')
    
        objFileAnalyst = CFileAnalyst_tollguide(f_tollguide)
        listrecord = objFileAnalyst.analyse()

        for record in listrecord:
            self.pg.insert('road_tollguide', 
                          (
                           'mesh_code',
                           'node_id',
                           'toll_type',
                           'toll_num',
                           'roads',
                          ),
                          (
                           record[0],
                           record[1],
                           record[2],
                           record[3],
                           record[4]
                          )
                          )
        
        self.pg.commit2()
        
        return 0    
    
    def _Do_ferry_code(self,sf_path):
        pg = self.pg
        pgcur = self.pg.pgcur2
        
        f_ferry_code = io.open(os.path.join(sf_path, 'ferry.code.doc'), 'r', 8192, 'euc-jp')
         
        pgcur.copy_from(f_ferry_code, 'road_ferry_code', '\t', "", 8192, ('type','ferry_id','ferry_name'))
        pg.commit2()
        f_ferry_code.close()
          
        sqlcmd = """

              select deal_type_of_sidefile('road_ferry_code');

                """
        
        if self.pg.execute2(sqlcmd) == -1:
            return -1
        else:
            self.pg.commit2()
                    
        return 0

    def _Do_gs(self,sf_path):
        
        pg = self.pg
        pgcur = self.pg.pgcur2
        
        f_gs = io.open(os.path.join(sf_path, 'gs.txt'), 'r', 8192, 'euc-jp')
         
        pgcur.copy_from(f_gs, 'road_gs', '\t', "", 8192, ('road_code','road_point','side_f','chain_id','open_time','sunday','saturday','friday','thursday','wednesday','tuesday','monday','obon_day','year_end','year_start','holiday','shukusai'))
        pg.commit2()
        f_gs.close()
          
        return 0

    def _Do_multiple_store(self,sf_path):
        
        pg = self.pg
        pgcur = self.pg.pgcur2
        
        f_multiple_store = io.open(os.path.join(sf_path, 'multiple_store.txt'), 'r', 8192, 'euc-jp')
         
        pgcur.copy_from(f_multiple_store, 'road_multiple_store', '\t', "", 8192, ('road_code','road_point','side_f','chain_id','open_time','sunday','saturday','friday','thursday','wednesday','tuesday','monday','obon_day','year_end','year_start','holiday','shukusai'))
        pg.commit2()
        f_multiple_store.close()
          
        return 0

    def _Do_road_code(self,sf_path):
        pg = self.pg
        pgcur = self.pg.pgcur2
        
        f_road_code = io.open(os.path.join(sf_path, 'road.code.doc'), 'r', 8192, 'euc-jp')
         
        pgcur.copy_from(f_road_code, 'road_road_code', '\t', "", 8192, ('type','road_code','road_name','road_yomi'))
        pg.commit2()
        f_road_code.close()
          
        sqlcmd = """
             
              select deal_type_of_sidefile('road_road_code');

                """
        
        if self.pg.execute2(sqlcmd) == -1:
            return -1
        else:
            self.pg.commit2()
                    
        return 0

    def _Do_road_code_highway(self,sf_path):
        pg = self.pg
        pgcur = self.pg.pgcur2
        
        f_road_code_highway = io.open(os.path.join(sf_path, 'road.code.doc.highway'), 'r', 8192, 'euc-jp')
         
        pgcur.copy_from(f_road_code_highway, 'road_road_code_highway', '\t', "", 8192, ('type','road_code','road_name','road_yomi'))
        pg.commit2()
        f_road_code_highway.close()
          
        sqlcmd = """
              
              select deal_type_of_sidefile('road_road_code_highway');

                """
        
        if self.pg.execute2(sqlcmd) == -1:
            return -1
        else:
            self.pg.commit2()
                    
        return 0

    def _Do_roadpoint(self,sf_path):
        
        pg = self.pg
        pgcur = self.pg.pgcur2
        
        f_roadpoint = io.open(os.path.join(sf_path, 'roadpoint.txt'), 'r', 8192, 'euc-jp')
         
        pgcur.copy_from(f_roadpoint, 'road_roadpoint', '\t', "", 8192, ('point_mesh_code','point_x','point_y','mesh_id','fac_mesh_code','fac_x','fac_y','addr_id','road_point','fac_name','fac_yomi','road_code','fac_type','point_pos','point_name','point_yomi','sa_pa','name_f','fac_f'))
        pg.commit2()
        f_roadpoint.close()
  
        sqlcmd = """
                select AddGeometryColumn('road_roadpoint', 'geom', 4326, 'POINT', 2);                
                update road_roadpoint set geom = create_geom_of_sidefile(fac_mesh_code,fac_x,fac_y);
                
                """
        
        if self.pg.execute2(sqlcmd) == -1:
            return -1
        else:
            self.pg.commit2()
                                     
        return 0

    def _Do_roadpoint_normal(self,sf_path):
        
        pg = self.pg
        pgcur = self.pg.pgcur2
        
        f_roadpoint_normal = io.open(os.path.join(sf_path, 'roadpoint_normal.txt'), 'r', 8192, 'euc-jp')
         
        pgcur.copy_from(f_roadpoint_normal, 'road_roadpoint_normal', '\t', "", 8192, ('point_mesh_code','point_x','point_y','mesh_id','fac_mesh_code','fac_x','fac_y','addr_id','road_point','fac_name','fac_yomi','road_code','fac_type','point_pos','point_name','point_yomi','sa_pa','name_f','fac_f'))
        pg.commit2()
        f_roadpoint_normal.close()
  
        sqlcmd = """
                select AddGeometryColumn('road_roadpoint_normal', 'geom', 4326, 'POINT', 2);                
                update road_roadpoint_normal set geom = create_geom_of_sidefile(fac_mesh_code,fac_x,fac_y);
                
                """
        
        if self.pg.execute2(sqlcmd) == -1:
            return -1
        else:
            self.pg.commit2()
                                     
        return 0

    def _Do_roadpoint_bunnki(self,sf_path):
        
        pg = self.pg
        pgcur = self.pg.pgcur2
        
        f_roadpoint_bunnki = io.open(os.path.join(sf_path, 'roadpoint.bunnki.txt'), 'r', 8192, 'euc-jp')
         
        pgcur.copy_from(f_roadpoint_bunnki, 'road_roadpoint_bunnki', '\t', "", 8192, ('point_mesh_code','point_x','point_y','mesh_id','fac_mesh_code','fac_x','fac_y','addr_id','road_point','fac_name','fac_yomi','road_code','fac_type','point_pos','point_name','point_yomi','sa_pa','name_f','fac_f'))
        pg.commit2()
        f_roadpoint_bunnki.close()
  
        sqlcmd = """
                select AddGeometryColumn('road_roadpoint_bunnki', 'geom', 4326, 'POINT', 2);                
                update road_roadpoint_bunnki set geom = create_geom_of_sidefile(fac_mesh_code,fac_x,fac_y);
                
                """
        
        if self.pg.execute2(sqlcmd) == -1:
            return -1
        else:
            self.pg.commit2()
                                     
        return 0

    def _Do_store_code(self,sf_path):
        
        pg = self.pg
        pgcur = self.pg.pgcur2
        
        f_store_code = io.open(os.path.join(sf_path, 'store_code.txt'), 'r', 8192, 'euc-jp')
         
        pgcur.copy_from(f_store_code, 'road_store_code', '\t', "", 8192, ('chain_id','chain_name'))
        pg.commit2()
        f_store_code.close()
          
        return 0

    def _Do_connectway(self,cw_path):

        pg = self.pg
        pgcur = self.pg.pgcur2
                
        sqlcmd = ' DROP TABLE IF EXISTS road_connectway;\
                CREATE TABLE road_connectway \
                (   \
                  gid int NOT NULL, \
                  cw_num int NOT NULL, \
                  dir smallint,\
                  type smallint,\
                  node_num smallint,\
                  meshcode integer,\
                  nodeid integer\
                );'

        if self.pg.execute2(sqlcmd) == -1:
            return -1
        else:
            self.pg.commit2()  
               
        objFileAnalyst = CFileAnalyst_connectway(cw_path)
        objFileAnalyst.analyse()
          
        f_io_obj = io.open('load_bin_sql.sql', 'rt', 8192)
        
        pgcur.copy_from(f_io_obj, 'road_connectway', '\t', "", 8192, None)
        pg.commit2()
        f_io_obj.close()        

class CFileAnalyst_connectway: 
    
    def __init__(self, DataPath):
        self.rootpath = DataPath   
                
    def _analyseLine(self, file_name):
        file_obj = open(file_name, 'rb')
        cw_cnt = common.common_func.get_int_value(file_obj, 4)
        
        self.__cur_gid = 0
        for i in range(0, cw_cnt):
            cw_class_node_num = common.common_func.get_int_value(file_obj, 4)

            type = (cw_class_node_num >> 16) & 255
            dir = (cw_class_node_num >> 24) & 255           
            node_num = cw_class_node_num & 65535
            
            for j in range(0, node_num):
                mesh = common.common_func.get_int_value(file_obj, 4)              
                nodeid = common.common_func.get_int_value(file_obj, 4)
         
                self.__cur_gid = self.__cur_gid + 1
                temp_str = "%d\t%d\t%d\t%d\t%d\t%d\t%d\n" %(self.__cur_gid, i, dir, type, j, mesh, nodeid)
                
                self.__output_sql_file_handle.write(temp_str)
        
        file_obj.close()
        
    def analyse(self):
 
        self.__output_sql_file_handle = open('load_bin_sql.sql', 'w')        
        if self.__output_sql_file_handle is None:
            return -1 
                
        listsub = os.listdir(self.rootpath)
        for sub in listsub:
            subpath = os.path.join(self.rootpath, sub)
            if os.path.isfile(subpath):
                self._analyseLine(subpath)
            else:
                self.analyse(subpath)                
        
        self.__output_sql_file_handle.close()  
                                       
class CFileAnalyst_tollguide:
    def __init__(self, DataPath):
        self.rootpath = DataPath
    
    def analyse(self):
        listrecord = []
        
        filepath = self.rootpath
        objFile = open(filepath)
        listline = objFile.readlines()
        objFile.close()
        
        for line in listline:
            line = line.strip()
            if line:
                record = self._analyseLine(line)
                if record:
                    listrecord.append(record)
    
        return listrecord
   
    def _analyseLine(self, line):
        record = []
        
        listrow = line.split("\t")
        
        # mesh_code
        record.append(listrow[0])
        
        # node_id
        record.append(listrow[1])
        
        # toll_type
        record.append(listrow[2])
        
        # toll_num
        record.append(listrow[3])
              
        # roads
        record_small = []
        record_out = []
        num = ( len(listrow) - 3 ) / 2
        for i in range(num):
            array = []
            for j in range(1,3):       
                element = listrow[3 + 2*i + j]
                array.append(element)
            record_small = ','.join(array)
            record_out.append(record_small)
            
        record_big = '|'.join(record_out)    
        record.append(record_big)
        
        return record
        
class CFileAnalyst_highway:
    def __init__(self, DataPath):
        self.rootpath = DataPath
    
    def analyse(self):
        listrecord = []
        
        filepath = self.rootpath
        objFile = open(filepath)
        listline = objFile.readlines()
        objFile.close()
        
        for line in listline:
            line = line.strip()
            if line:
                record = self._analyseLine(line)
                if record:
                    listrecord.append(record)
    
        return listrecord
   
    def _analyseLine(self, line):
        record = []
        
        listrow = line.split("\t")
        
        # road_code
        record.append(listrow[0])
        
        # linkcls_c
        record.append(listrow[1])
        
        # dir_f
        record.append(listrow[2])
        
        # dir_no
        record.append(listrow[3])

        # road_no
        record.append(listrow[4])
        
        # next_no
        record.append(listrow[5])
        
        # next_dis
        record.append(listrow[6])
        
        # end_f
        record.append(listrow[7])  

        # forward_num
        record.append(listrow[8])  
        
        # backward_num
        record.append(listrow[9])  
              
        # branchlist
        record_small = []
        record_out = []
        num = ( len(listrow) - 9 ) / 5
        for i in range(num):
            array = []
            for j in range(1,6):       
                element = listrow[9 + 5*i + j]
                array.append(element)
            record_small = ','.join(array)
            record_out.append(record_small)
            
        record_big = '|'.join(record_out)    
        record.append(record_big)
        
        return record
    
class CFileAnalyst_hwysame:
    def __init__(self, DataPath):
        self.rootpath = DataPath
    
    def analyse(self):
        listrecord = []
        
        filepath = self.rootpath
        objFile = open(filepath)
        listline = objFile.readlines()
        objFile.close()
        
        for line in listline:
            line = line.strip()
            if line:
                record = self._analyseLine(line)
                if record:
                    listrecord.append(record)
    
        return listrecord
   
    def _analyseLine(self, line):
        record = []
        
        listrow = line.split("\t")
        
        # road_code
        record.append(listrow[0])
        
        # road_no
        record.append(listrow[1])
        
        # dir_f
        record.append(listrow[2])
        
        # name
        #record.append(listrow[3])
        #print listrow[0]
        #print listrow[1]
        #print listrow[2]
        #print listrow[3]
        
        record.append(listrow[3].decode('euc-jp','ignore').encode('utf-8')) 
             
        # roads
        record_out = []
        num = len(listrow) - 4
        array = []
                    
        for i in range(num):

            element = listrow[3 + i + 1]
            array.append(element)
        record_out = '|'.join(array)
   
        record.append(record_out)
        
        return record