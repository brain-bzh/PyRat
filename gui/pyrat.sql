/**********************************************************************************************************************************************************************************************************************************/
/************************************************************************************************************* LICENSE ************************************************************************************************************/
/**********************************************************************************************************************************************************************************************************************************/
--
--    Copyright © 2016 Bastien Pasdeloup (name.surname@gmail.com) and Télécom Bretagne
--
--    This file is part of PyRat.
--
--    PyRat is free software: you can redistribute it and/or modify
--    it under the terms of the GNU General Public License as published by
--    the Free Software Foundation, either version 3 of the License, or
--    (at your option) any later version.
--
--    PyRat is distributed in the hope that it will be useful,
--    but WITHOUT ANY WARRANTY; without even the implied warranty of
--    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--    GNU General Public License for more details.
--
--    You should have received a copy of the GNU General Public License
--    along with PyRat.  If not, see <http:--www.gnu.org/licenses/>.
--
/**********************************************************************************************************************************************************************************************************************************/
/************************************************************************************************************ DATABASE ************************************************************************************************************/
/**********************************************************************************************************************************************************************************************************************************/

    -- Creation
    CREATE DATABASE IF NOT EXISTS `pyrat`;
    
    -- Selection
    USE `pyrat`;

/**********************************************************************************************************************************************************************************************************************************/
/************************************************************************************************************** USERS *************************************************************************************************************/
/**********************************************************************************************************************************************************************************************************************************/

    -- Creation
    CREATE USER IF NOT EXISTS 'pyrat'@'localhost'
    IDENTIFIED BY 'pyrat';
    
    -- Privileges
    GRANT SELECT, INSERT, UPDATE, DELETE
    ON *.*
    TO 'pyrat'@'localhost';
    
/**********************************************************************************************************************************************************************************************************************************/
/************************************************************************************************************* TABLES *************************************************************************************************************/
/**********************************************************************************************************************************************************************************************************************************/
    
    -- Game
    CREATE TABLE IF NOT EXISTS `game`
    (
        
        -- Fields
        `id`                    int         NOT NULL    AUTO_INCREMENT,
        `start`                 datetime    NOT NULL,
        `map`                   text        NOT NULL,
        `piecesOfCheese`        text        NOT NULL,
        `width`                 int         NOT NULL,
        `height`                int         NOT NULL,
        
        -- Primary key
        PRIMARY KEY (`id`)
        
    );
    
/**********************************************************************************************************************************************************************************************************************************/
    
    -- Player
    CREATE TABLE IF NOT EXISTS `player`
    (
    
        -- Fields
        `game_id`               int         NOT NULL,
        `number`                int         NOT NULL,
        `name`                  text        NOT NULL,
        `startingLocation`      text        NOT NULL,
        
        -- Primary key
        PRIMARY KEY (`game_id`, `number`),
        
        -- Foreing keys
        FOREIGN KEY (`game_id`) REFERENCES `game` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
        
    );

/**********************************************************************************************************************************************************************************************************************************/

    -- Move
    CREATE TABLE IF NOT EXISTS `move`
    (
    
        -- Fields
        `game_id`               int         NOT NULL,
        `player_number`         int         NOT NULL,
        `time`                  int         NOT NULL,
        `duration`              int         NOT NULL,
        `direction`             char        NOT NULL,
        `scoreAfter`            double      NOT NULL,
        `cheeseToRemoveAfter`   text        NOT NULL,
        
        -- Primary key
        PRIMARY KEY (`game_id`, `player_number`, `time`),
        
        -- Foreing keys
        FOREIGN KEY (`game_id`,`player_number`) REFERENCES `player` (`game_id`, `number`) ON DELETE CASCADE ON UPDATE CASCADE
        
    );
    
/**********************************************************************************************************************************************************************************************************************************/
/************************************************************************************************************* EVENTS *************************************************************************************************************/
/**********************************************************************************************************************************************************************************************************************************/

    -- Clean the entries older than 24 hours
    DELIMITER $$
    CREATE EVENT IF NOT EXISTS `event_clean_old`
    ON SCHEDULE
    EVERY 1 MINUTE
    ENABLE
    DO
        DELETE FROM game
        WHERE game.start < DATE_SUB(CONCAT(CURDATE(), ' 00:00:00'), INTERVAL 1 DAY)
    $$
    DELIMITER ;

/**********************************************************************************************************************************************************************************************************************************/
/**********************************************************************************************************************************************************************************************************************************/