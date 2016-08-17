CREATE TABLE `users` (
  `UID` int(11) NOT NULL AUTO_INCREMENT,
  `UserID` varchar(45) DEFAULT NULL,
  `PassWord` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`UID`),
  UNIQUE KEY `UserID_UNIQUE` (`UserID`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8;


DROP PROCEDURE InsertUsers;

DELIMITER //
CREATE PROCEDURE InsertUsers(IN userId varchar(45), IN pass varchar(45))
BEGIN
	INSERT INTO test.users SET UserID = userId, PassWord = pass;
END //
DELIMITER ;

CALL InsertUsers('aaa', '2222');



DROP PROCEDURE CheckUsers;

DELIMITER //
CREATE PROCEDURE CheckUsers(IN id varchar(45), IN pass varchar(45))
BEGIN
	DECLARE n INT DEFAULT 0;
    SELECT COUNT(*) INTO n FROM test.users WHERE (`UserID` = id AND `PassWord` = pass);
    IF n = 0 THEN
		SIGNAL SQLSTATE 'ERROR';
	END IF;
END //
DELIMITER ;

CALL CheckUsers('aa222', '1234');


DROP PROCEDURE DeleteUsers;

DELIMITER //
CREATE PROCEDURE DeleteUsers(IN id varchar(45), IN pass varchar(45))
BEGIN
	DECLARE n INT DEFAULT 0;
    SELECT COUNT(*) INTO n FROM test.users WHERE (`UserID` = id AND `PassWord` = pass);
    IF n = 0 THEN
		SIGNAL SQLSTATE 'ERROR';
	ELSE
		DELETE FROM test.users WHERE (`UserID` = userId AND `PassWord` = pass);
	END IF;
END //
DELIMITER ;

CALL DeleteUsers('aa222', '1234');