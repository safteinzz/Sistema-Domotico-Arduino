CREATE DATABASE DBPIng;

use DBPing;

drop table if exists Log_Detecciones;

CREATE TABLE IF NOT EXISTS Log_Detecciones(
	id MEDIUMINT unsigned not null AUTO_INCREMENT,
    log DATETIME,
    primary key (id)
) ENGINE=innoDB;


CREATE TABLE temperatura (
	fechaHora timestamp DEFAULT CURRENT_TIMESTAMP,
	temperatura VARCHAR(200),
	temperaturaSeteada VARCHAR(200)
);

/* Tests
use DBPing;
select * from Log_Detecciones;
insert into Log_Detecciones(log) values ("2019-05-09 17:50:24");
*/
