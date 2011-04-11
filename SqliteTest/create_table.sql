drop table if exists continent_mapping;
drop table if exists country_mapping;
drop table if exists state_mapping;
drop table if exists city_mapping;
drop table if exists host_mapping;
drop table if exists url_mapping;
drop table if exists attack_mapping;
drop table if exists main_log;

create table continent_mapping (
    id int1 primary key asc,
    name character(20) not null
);
     
create table country_mapping (
    id int2 primary key asc,
    name character(40) not null
);

create table state_mapping (
    id int4 primary key asc,
    name character(40) not null
);

create table city_mapping (
    id int4 primary key asc,
    name character(40) not null
);

create table host_mapping (
    id int4 primary key asc,
    name character(128) not null
);

create index host_index on host_mapping(name);

create table url_mapping (
    id integer primary key autoincrement,
    name character(256) not null
);

create index url_index on url_mapping(name);

create table attack_mapping (
    id int2 primary key asc,
    name character(64) not null
);

create index attack_index on attack_mapping(name);

create table main_log (
    id int8 primary key asc,
    timestamp datetime not null,
    src_ip int4 not null,
    src_port int2 not null,
    dst_ip int4 not null,
    dst_port int2 not null,
    bandwidth int2 not null,
    
    /* don't use foreign key for now */
    continent_id int1 not null,
    country_id int2 not null,
    state_id int4,
    city_id int4,
    host_id int4 not null,
    url_id int4 not null,
    attack_id int2 not null
);

    
    
    
    
