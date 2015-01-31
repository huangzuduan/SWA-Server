#
# DB: wx1_log
#

CREATE TABLE `platform_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `platformId` char(30) NOT NULL DEFAULT '' COMMENT '平台id',
  `zoneId` int(4) NOT NULL DEFAULT '0' COMMENT '游戏服id',
  `uid` int(4) NOT NULL DEFAULT '0' COMMENT '玩家UID',
  `status` tinyint(3) NOT NULL DEFAULT '0' COMMENT '状态',
  `mac` char(20) NOT NULL DEFAULT '' COMMENT 'MAC地址',
  `ip` char(20) NOT NULL DEFAULT '' COMMENT '玩家IP',
  `device` char(10) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

