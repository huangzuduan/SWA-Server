-- --------------------------------------------------------
-- 主机:                           127.0.0.1
-- 服务器版本:                        5.5.5-10.0.15-MariaDB - mariadb.org binary distribution
-- 服务器操作系统:                      Win64
-- HeidiSQL 版本:                  8.3.0.4694
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- 导出 swa_data 的数据库结构
DROP DATABASE IF EXISTS `swa_data`;
CREATE DATABASE IF NOT EXISTS `swa_data` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `swa_data`;


-- 导出  表 swa_data.account 结构
DROP TABLE IF EXISTS `account`;
CREATE TABLE IF NOT EXISTS `account` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` char(32) NOT NULL,
  `password` char(32) NOT NULL,
  `create_time` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UNIQUE_USERNAME` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- 正在导出表  swa_data.account 的数据：~0 rows (大约)
/*!40000 ALTER TABLE `account` DISABLE KEYS */;
INSERT INTO `account` (`id`, `username`, `password`, `create_time`) VALUES
	(1, 'hzd', 'hzd123', 1);
/*!40000 ALTER TABLE `account` ENABLE KEYS */;


-- 导出  表 swa_data.character 结构
DROP TABLE IF EXISTS `character`;
CREATE TABLE IF NOT EXISTS `character` (
  `character_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `account_id` int(11) NOT NULL,
  `nick` char(32) NOT NULL,
  `type` int(11) NOT NULL,
  `level` int(11) NOT NULL,
  PRIMARY KEY (`character_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 正在导出表  swa_data.character 的数据：~0 rows (大约)
/*!40000 ALTER TABLE `character` DISABLE KEYS */;
/*!40000 ALTER TABLE `character` ENABLE KEYS */;


-- 导出  表 swa_data.login_status 结构
DROP TABLE IF EXISTS `login_status`;
CREATE TABLE IF NOT EXISTS `login_status` (
  `account_id` int(11) NOT NULL,
  `srv_zone_id` int(11) NOT NULL,
  `login_time` datetime NOT NULL,
  KEY `FK_login_status_account` (`account_id`),
  CONSTRAINT `FK_login_status_account` FOREIGN KEY (`account_id`) REFERENCES `account` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 正在导出表  swa_data.login_status 的数据：~0 rows (大约)
/*!40000 ALTER TABLE `login_status` DISABLE KEYS */;
/*!40000 ALTER TABLE `login_status` ENABLE KEYS */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
