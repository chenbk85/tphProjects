--------------------------------------------------------------------------------------- 
-- create index 
--------------------------------------------------------------------------------------- 

----------------------------------------------------- 
CREATE INDEX org_trfcsign_mapid_inlinkid_idx
  ON org_trfcsign
  USING btree
  (cast(mapid as bigint), cast(inlinkid as bigint));

----------------------------------------------------- 
CREATE INDEX org_trfcsign_mapid_nodeid_idx
  ON org_trfcsign
  USING btree
  (cast(mapid as bigint), cast(nodeid as bigint));

----------------------------------------------------- 
CREATE INDEX org_trfcsign_crid_idx
  ON org_trfcsign
  USING btree
  (cast(crid as bigint));

----------------------------------------------------- 
CREATE INDEX org_n_mapid_id_idx
  ON org_n
  USING btree
  (cast(mapid as bigint), cast(id as bigint));

----------------------------------------------------- 
CREATE INDEX org_r_mapid_id_idx
  ON org_r
  USING btree
  (cast(mapid as bigint), cast(id as bigint));

----------------------------------------------------- 
CREATE INDEX org_cr_crid_idx
  ON org_cr
  USING btree
  (cast(crid as bigint));

----------------------------------------------------- 
CREATE INDEX org_cond_mapid_condid_idx
  ON org_cond
  USING btree
  (cast(mapid as bigint), cast(condid as bigint));

----------------------------------------------------- 
CREATE INDEX org_cnl_mapid_condid_idx
  ON org_cnl
  USING btree
  (cast(mapid as bigint), cast(condid as bigint));

----------------------------------------------------- 
CREATE INDEX org_cnl_mapid_linkid_idx
  ON org_cnl
  USING btree
  (cast(mapid as bigint), cast(linkid as bigint));

----------------------------------------------------- 
CREATE INDEX org_cnl_mapid_nodeid_idx
  ON org_cnl
  USING btree
  (cast(mapid as bigint), cast(nodeid as bigint));

----------------------------------------------------- 
CREATE INDEX org_r_mapid_id_idx
  ON org_r
  USING btree
  (cast(mapid as bigint), cast(id as bigint));

----------------------------------------------------- 
CREATE INDEX org_n_mapid_id_idx
  ON org_n
  USING btree
  (cast(mapid as bigint), cast(id as bigint));

----------------------------------------------------- 
CREATE INDEX org_cond_crid_idx
  ON org_cond
  USING btree
  (crid);

----------------------------------------------------- 
CREATE INDEX org_cr_crid_idx
  ON org_cr
  USING btree
  (crid);