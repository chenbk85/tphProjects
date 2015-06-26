#encoding=utf-8
import os.path
import struct
import psycopg2
import sys

#==============================================================================
# comp_picture
#==============================================================================
class comp_picture(object):
    ''' picture entity'''
    def __init__(self):
        self.gid = 0
        self.dayName = None
        self.nightName = None
        self.arrow = None
        self.arrows = []

    def getGid(self):
        return self.gid

    def setGid(self, gid):
        self.gid = gid

    def getDayName(self):
        return self.dayName

    def setDayName(self, dayName):
        self.dayName = dayName

    def getNightName(self):
        return self.nightName

    def setNightName(self, nightName):
        self.nightName = nightName

    def getArrow(self):
        return self.arrow

    def setArrow(self, arrow):
        self.arrow = arrow

    def getArrows(self):
        return self.arrows

    def setArrows(self, arrow):
        self.arrows.append(arrow)

#==============================================================================
# comp_picture
#==============================================================================
class GeneratorPicBinary_Here(object):

    def __init__(self, host, dbname, user, password):
        constr = "host='" + host + "' dbname='" + dbname + \
                 "' user='" + user + "' password='" + password + "'"
        self.conn = psycopg2.connect(constr)    
        self.pgcur2 = self.conn.cursor()
    
    def __del__(self):
        self.pgcur2.close()
        self.conn.close()

    def select_gjv_Data(self):
        self.pgcur2.execute('''SELECT distinct dp_node_id, filename, side
                               FROM rdfpl_gjv_lat_display_org;''')
        rows = self.pgcur2.fetchall()
        pics = []
        i = 1
        for row in rows:
            pic = comp_picture()
            pic.setGid(i)
            pic.setDayName(row[1])
            pic.setNightName(row[1])
            pic.setArrow(row[2])
            pics.append(pic)
            i = i + 1

        self.pgcur2.execute('''SELECT distinct dp_node_id, filename, side
                                FROM rdfpl_all_jv_lat_display_org
                                where filename is not null and ejv_filename is null;''')
        allrows = self.pgcur2.fetchall()

        for row in allrows:
            pic = comp_picture()
            pic.setGid(i)
            pic.setDayName(row[1])
            pic.setNightName(row[1])
            pic.setArrow(row[2])
            pics.append(pic)
            i = i + 1
        return pics

    def select_ejv_Data(self):
        self.pgcur2.execute('''
            select ejv_filename,array_agg(outlinkid)
            from(
                SELECT distinct dp_node_id, ejv_filename,
                    case when jv_dest_link is null or jv_dest_link = dest_link_id then dest_link_id
                        when jv_dest_link is not null and jv_dest_link <> dest_link_id then jv_dest_link
                    else 0
                    end as outlinkid
                FROM rdfpl_all_jv_lat_display_org
                where ejv_filename is not null
            ) as a
            group by ejv_filename;''')
        '''and originating_link_id in (816431040,116164590)'''
        rows = self.pgcur2.fetchall()
        pics = []
        i = 1
        for row in rows:
            pic = comp_picture()
            pic.setGid(i)
            pic.setDayName(row[0])
            pic.setNightName(row[0])
            for arrow in row[1]:
                pic.setArrows(arrow)
            pics.append(pic)
            i = i + 1
        return pics

    def select_ejv_condition_Data(self):
        self.pgcur2.execute('''
                select a.file_name,array_agg(outlink)
                from
                (
                    select distinct E.file_name, F.all_links[array_upper(F.all_links,1)] as outlink
                    from
                    (
                        select A.condition_id, A.condition_type, A.nav_strand_id, A.access_id,
                        B.file_id, C.file_type,C.file_name from
                        (
                            select condition_id, condition_type, nav_strand_id, access_id
                            from rdf_condition
                            where condition_type = 20
                        ) as A left join RDF_FILE_FEATURE as B
                        on A.condition_id = B.feature_id left join rdf_file as C
                        on B.file_id = C.file_id
                        where file_type = 34
                    ) as E left join
                    (
                        select nav_strand_id, array_agg(link_id) as all_links, array_agg(node_id) as all_nodes
                        from
                        (
                            select nav_strand_id, seq_num, link_id, node_id
                            from rdf_nav_strand
                            where nav_strand_id in
                            (
                                SELECT nav_strand_id
                                FROM rdf_condition
                                where condition_type = 20
                            ) order by nav_strand_id, seq_num
                        ) as D group by nav_strand_id
                    ) as F on E.nav_strand_id = F.nav_strand_id
                ) as a
                group by a.file_name;''')
        '''and originating_link_id in (816431040,116164590)'''
        rows = self.pgcur2.fetchall()
        pics = []
        i = 1
        for row in rows:
            pic = comp_picture()
            pic.setGid(i)
            pic.setDayName(row[0])
            pic.setNightName(row[0])
            for arrow in row[1]:
                pic.setArrows(arrow)
            pics.append(pic)
            i = i + 1
        return pics

    def makeGJunctionResultTable(self, dirFileDir, destFileDir):
        pictures = self.select_gjv_Data()
        for pic in pictures:
            pic_name_strs = (pic.getDayName()).split('/')
            pic_type = pic_name_strs[0]

            picname_bin = os.path.splitext(pic_name_strs[2])[0]
            picDirPath = ''
            arrow_file_name = ''
            side = pic.getArrow()
            if pic_type == 'SDPS':
                picDirPath = dirFileDir + "\\" + "SDPS"
                if side == 'R':
                    arrow_file_name = picname_bin + '_lane1'
                elif side == 'L':
                    arrow_file_name = picname_bin + '_lane2'
                else:
                    print "SDPS side wrong!"
            elif pic_type == 'MDPS':
                picDirPath = dirFileDir + "\\" + "MDPS"
                if side == 'M':
                    arrow_file_name = picname_bin + '_lane1'
                elif side == 'R':
                    arrow_file_name = picname_bin + '_lane2'
                elif side == 'L':
                    arrow_file_name = picname_bin + '_lane3'
                else:
                    print "MDPS side wrong!!"
            else:
                print "pic_type wrong!"

            destFile = destFileDir + "\\" + picname_bin.lower() + ".dat"
            arrowFile = destFileDir + "\\" + arrow_file_name.lower() + ".dat"
            if os.path.isdir(picDirPath):
                # day and nigth illust
                if  os.path.isfile(destFile) == False:
                    dayPicPath = os.path.join(picDirPath, "DAY", picname_bin + ".jpg")
                    nightPicPath = os.path.join(picDirPath, "NIGHT", picname_bin + ".jpg")
                    dayFis = open(dayPicPath, 'rb')
                    nightFis = open(nightPicPath, 'rb')
                    fos = open(destFile, 'wb')
                    dayPicLen = os.path.getsize(dayPicPath)
                    nightPicLen = os.path.getsize(nightPicPath)
                    headerBuffer = struct.pack("<HHbiibii", 0xFEFE, 2, 1, 22, \
                                               dayPicLen, 2, 22 + dayPicLen, \
                                               nightPicLen)
                    resultBuffer = headerBuffer + dayFis.read() \
                                            + nightFis.read()
                    dayFis.close()
                    nightFis.close()
                    fos.write(resultBuffer)
                    fos.close()
                    # ARROW PIC BUILD
                if os.path.isfile(arrowFile) == False:
                    picPathFile = picDirPath + "\\DAY\\" + arrow_file_name + ".png"
                    arrowFis = open(picPathFile, 'rb')
                    fos = open(arrowFile, 'wb')
                    arrowPicLen = os.path.getsize(picPathFile)
                    headerBuffer = struct.pack("<HHbii", 0xFEFE, 1, 0, 13, arrowPicLen)
                    resultBuffer = headerBuffer + arrowFis.read()
                    arrowFis.close()
                    fos.write(resultBuffer)
                    fos.close()
                    
                    
    def makeGJunctionResultTable_17cy(self, dirFileDir, destFileDir):
        pictures = self.select_gjv_Data()
        for pic in pictures:
            pic_name_strs = (pic.getDayName()).split('/')
            pic_type = pic_name_strs[0]

            picname_bin = os.path.splitext(pic_name_strs[2])[0]
            picDirPath = ''
            arrow_file_name = ''
            side = pic.getArrow()
            if pic_type == 'SDPS':
                picDirPath = dirFileDir + "\\" + "SDPS"
                if side == 'R':
                    arrow_file_name = picname_bin + '_lane1'
                elif side == 'L':
                    arrow_file_name = picname_bin + '_lane2'
                else:
                    print "SDPS side wrong!"
            elif pic_type == 'MDPS':
                picDirPath = dirFileDir + "\\" + "MDPS"
                if side == 'M':
                    arrow_file_name = picname_bin + '_lane1'
                elif side == 'R':
                    arrow_file_name = picname_bin + '_lane2'
                elif side == 'L':
                    arrow_file_name = picname_bin + '_lane3'
                else:
                    print "MDPS side wrong!!"
            else:
                print "pic_type wrong!"

            destFile = destFileDir + "\\" + picname_bin.lower() + ".dat"
            arrowFile = destFileDir + "\\" + arrow_file_name.lower() + ".dat"
            if os.path.isdir(picDirPath):
                # day and nigth illust
                if  os.path.isfile(destFile) == False:
                    dayPicPath = os.path.join(picDirPath, "DAY", picname_bin + ".jpg")
                    nightPicPath = os.path.join(picDirPath, "NIGHT", picname_bin + ".jpg")
                    dayFis = open(dayPicPath, 'rb')
                    nightFis = open(nightPicPath, 'rb')
                    fos = open(destFile, 'wb')
                    dayPicLen = os.path.getsize(dayPicPath)
                    nightPicLen = os.path.getsize(nightPicPath)
                    headerBuffer = struct.pack("<HHbiibii", 0xFEFE, 2, 1, 22, \
                                               dayPicLen, 2, 22 + dayPicLen, \
                                               nightPicLen)
                    resultBuffer = headerBuffer + dayFis.read() + nightFis.read()
                    dayFis.close()
                    nightFis.close()
                    fos.write(resultBuffer)
                    fos.close()
                    # ARROW PIC BUILD
                if os.path.isfile(arrowFile) == False:
                    picPathFile = picDirPath + "\\DAY\\" + arrow_file_name + ".png"
                    arrowFis = open(picPathFile, 'rb')
                    fos = open(arrowFile, 'wb')
                    arrowPicLen = os.path.getsize(picPathFile)
                    headerBuffer = struct.pack("<HHbii", 0xFEFE, 1, 0, 13, arrowPicLen)
                    resultBuffer = headerBuffer + arrowFis.read()
                    arrowFis.close()
                    fos.write(resultBuffer)
                    fos.close()   
                                 
    # 首先处理driver pic
    # 如果driver pic中背景图或者箭头图原图不存在，那么用bird pic对应
    def makeEJunctionResultTable(self, dirFileDir, destFileDir, bUseCondition):
        pictures = None
        if bUseCondition == 'condition':
            pictures = self.select_ejv_condition_Data()
        else:
            pictures = self.select_ejv_Data()
        arrowdir = os.path.join(dirFileDir, "DAY")
        pics_unfinished = []
        for pic in pictures:
            exist_flag = 1
            pic_name_strs = pic.getDayName()
            back_picname = os.path.splitext(pic_name_strs)[0]
            destFile = destFileDir + "\\" + back_picname.lower() + ".dat"

            if os.path.isdir(dirFileDir):
                dayPicPath = ''
                nightPicPath = ''
                # day and nigth illust
                if  os.path.isfile(destFile) == False:
                    dayPicPath = os.path.join(dirFileDir, "DAY", back_picname + ".jpg")
                    nightPicPath = os.path.join(dirFileDir, "NIGHT", back_picname + ".jpg")
                    if not (os.path.exists(dayPicPath) and os.path.exists(nightPicPath)):
                        print dayPicPath, 'or', nightPicPath, 'driver not exist!!!'
                        pics_unfinished.append(pic)
                        exist_flag = 0
                        continue


                    # ARROW PIC BUILD
                dest_arrow_names = []
                source_arrow_names = []
                for arrow in pic.getArrows():
                    arrow_file_name = back_picname + '_' + str(arrow)
                    arrowFile = destFileDir + "\\" + arrow_file_name.lower() + ".dat"
                    if os.path.isfile(arrowFile) == False:
                        dest_arrow_names.append(arrowFile)
                        picPathFile = ''
                        for source_arrow in os.listdir(arrowdir):
                            if source_arrow.find(arrow_file_name) >= 0:
                                picPathFile = dirFileDir + "\\DAY\\" + source_arrow
                                source_arrow_names.append(picPathFile)

                        if not os.path.exists(picPathFile):
                            print picPathFile, 'driver arrow not exist!!!'
                            pics_unfinished.append(pic)
                            exist_flag = 0
                            break
                if exist_flag > 0:
                    dayFis = open(dayPicPath, 'rb')
                    nightFis = open(nightPicPath, 'rb')
                    b_fos = open(destFile, 'wb')
                    dayPicLen = os.path.getsize(dayPicPath)
                    nightPicLen = os.path.getsize(nightPicPath)
                    b_headerBuffer = struct.pack("<HHbiibii", 0xFEFE, 2, 1, 22, \
                                               dayPicLen, 2, 22 + dayPicLen, \
                                               nightPicLen)
                    b_resultBuffer = b_headerBuffer + dayFis.read() \
                                            + nightFis.read()
                    dayFis.close()
                    nightFis.close()
                    b_fos.write(b_resultBuffer)
                    b_fos.close()

                    for dest_arrow_name, source_arrow_name in zip(dest_arrow_names,source_arrow_names):
                        arrowFis = open(source_arrow_name, 'rb')
                        a_fos = open(dest_arrow_name, 'wb')
                        arrowPicLen = os.path.getsize(source_arrow_name)
                        a_headerBuffer = struct.pack("<HHbii", 0xFEFE, 1, 0, 13, arrowPicLen)
                        a_resultBuffer = a_headerBuffer + arrowFis.read()
                        arrowFis.close()
                        a_fos.write(a_resultBuffer)
                        a_fos.close()
        return pics_unfinished

    def makeBIRDJunctionResultTable(self, dirFileDir, destFileDir, pics):
        arrowdir = os.path.join(dirFileDir, "DAY")
        for pic in pics:
            pic_name_strs = pic.getDayName()
            back_picname = os.path.splitext(pic_name_strs)[0]
            destFile = destFileDir + "\\" + back_picname.lower() + ".dat"
            exist_flag = 1
            if os.path.isdir(dirFileDir):
                dayPicPath = ''
                nightPicPath = ''
                # day and nigth illust
                if  os.path.isfile(destFile) == False:
                    dayPicPath = os.path.join(dirFileDir, "DAY", back_picname + ".jpg")
                    nightPicPath = os.path.join(dirFileDir, "NIGHT", back_picname + ".jpg")
                    if not (os.path.exists(dayPicPath) and os.path.exists(nightPicPath)):
                        print dayPicPath, 'or', nightPicPath, 'bird not exist!!!'
                        exist_flag = 0
                        continue
                    # ARROW PIC BUILD
                dest_arrow_names = []
                source_arrow_names = []
                for arrow in pic.getArrows():
                    arrow_file_name = back_picname + '_' + str(arrow)
                    arrowFile = destFileDir + "\\" + arrow_file_name.lower() + ".dat"
                    if os.path.isfile(arrowFile) == False:
                        dest_arrow_names.append(arrowFile)
                        picPathFile = ''
                        for source_arrow in os.listdir(arrowdir):
                            if source_arrow.find(arrow_file_name) >= 0:
                                picPathFile = dirFileDir + "\\DAY\\" + source_arrow
                                source_arrow_names.append(picPathFile)

                        if not os.path.exists(picPathFile):
                            print picPathFile, 'bird arrow not exist!!!'
                            exist_flag = 0
                            break
                if exist_flag > 0:
                    dayFis = open(dayPicPath, 'rb')
                    nightFis = open(nightPicPath, 'rb')
                    b_fos = open(destFile, 'wb')
                    dayPicLen = os.path.getsize(dayPicPath)
                    nightPicLen = os.path.getsize(nightPicPath)
                    b_headerBuffer = struct.pack("<HHbiibii", 0xFEFE, 2, 1, 22, \
                                               dayPicLen, 2, 22 + dayPicLen, \
                                               nightPicLen)
                    b_resultBuffer = b_headerBuffer + dayFis.read() \
                                            + nightFis.read()
                    dayFis.close()
                    nightFis.close()
                    b_fos.write(b_resultBuffer)
                    b_fos.close()

                    for dest_arrow_name, source_arrow_name in zip(dest_arrow_names, source_arrow_names):
                        arrowFis = open(source_arrow_name, 'rb')
                        a_fos = open(dest_arrow_name, 'wb')
                        arrowPicLen = os.path.getsize(source_arrow_name)
                        a_headerBuffer = struct.pack("<HHbii", 0xFEFE, 1, 0, 13, arrowPicLen)
                        a_resultBuffer = a_headerBuffer + arrowFis.read()
                        arrowFis.close()
                        a_fos.write(a_resultBuffer)
                        a_fos.close()
                else:
                    print pic.getDayName()
        return 0

def main():
    arglen = len(sys.argv)
    if(arglen <= 2):
        return
    else:
        host = ""
        dbname = ""
        user = ""
        password = ""
        gjvsrcdir = ""
        gjvdestdir = ""
        ejvsrcdir = ""
        ejvdestdir = "" 
        ejvcondition = ""
        birdsrcdir = "" 
        birddestdir = ""
        for i in range(1, arglen):
            arg = sys.argv[i]
            if(arg == "/host"):
                host = str(sys.argv[i+1])
            elif(arg == "/dbname"):
                dbname = str(sys.argv[i+1])
            elif(arg == "/user"):
                user = str(sys.argv[i+1])
            elif(arg == "/password"):
                password = str(sys.argv[i+1])
            elif(arg == "/gjvsrcdir"):
                gjvsrcdir = str(sys.argv[i+1])
            elif(arg == "/gjvdestdir"):
                gjvdestdir = str(sys.argv[i+1])
            elif(arg == "/ejvsrcdir"):
                ejvsrcdir = str(sys.argv[i+1])
            elif(arg == "/ejvdestdir"):
                ejvdestdir = str(sys.argv[i+1]) 
            elif(arg == "/ejvcondition"):
                ejvcondition = str(sys.argv[i+1])
            elif(arg == "/birdsrcdir"):
                birdsrcdir = str(sys.argv[i+1]) 
            elif(arg == "/birddestdir"):
                birddestdir = str(sys.argv[i+1])
            else:
                pass
        test = GeneratorPicBinary_Here(host, dbname, 
                                       user, password)
        test.makeGJunctionResultTable(gjvsrcdir, gjvdestdir)
        pics = test.makeEJunctionResultTable(ejvsrcdir, ejvdestdir, ejvcondition)
        #test.makeBIRDJunctionResultTable(birdsrcdir, birddestdir, pics)
    return    


if __name__ == '__main__':
    main()