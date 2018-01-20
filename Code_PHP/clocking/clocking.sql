-- phpMyAdmin SQL Dump
-- version 4.6.4
-- https://www.phpmyadmin.net/
--
-- Gép: 127.0.0.1
-- Létrehozás ideje: 2018. Jan 20. 10:38
-- Kiszolgáló verziója: 5.7.14
-- PHP verzió: 5.6.25

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Adatbázis: `clocking`
--

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `events`
--

CREATE TABLE `events` (
  `clock_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `clocked_in` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `clocked_out` datetime DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `event_ended` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- A tábla adatainak kiíratása `events`
--

INSERT INTO `events` (`clock_id`, `user_id`, `clocked_in`, `clocked_out`, `event_ended`) VALUES
(53, 1, '2018-01-11 17:46:59', '2018-01-12 22:43:51', 1),
(54, 2, '2018-01-12 17:47:09', '2018-01-12 18:35:01', 1),
(55, 2, '2018-01-12 18:50:32', '2018-01-12 19:20:40', 1),
(56, 1, '2018-01-12 19:20:34', '2018-01-12 19:28:31', 1),
(57, 2, '2018-01-12 19:23:40', '2018-01-12 20:01:37', 1),
(58, 2, '2018-01-12 20:07:51', '2018-01-12 20:18:12', 1),
(61, 2, '2018-01-12 20:21:55', '2018-01-12 20:26:38', 1),
(62, 2, '2018-01-12 20:47:38', '2018-01-12 22:28:05', 1),
(63, 1, '2018-01-12 21:03:21', '2018-01-12 22:28:12', 1),
(71, 2, '2018-01-12 23:00:25', '2018-01-14 10:27:58', 1),
(72, 1, '2018-01-12 23:10:28', '2018-01-14 10:27:51', 1),
(73, 2, '2018-01-14 10:28:13', '2018-01-14 11:06:07', 1),
(74, 1, '2018-01-14 10:28:20', '2018-01-14 11:00:06', 1),
(77, 2, '2018-01-14 11:09:51', '2018-01-15 12:06:53', 1),
(78, 1, '2018-01-14 11:09:58', '2018-01-15 12:07:00', 1),
(79, 2, '2018-01-15 12:07:07', '2018-01-15 16:37:17', 1),
(80, 1, '2018-01-15 12:07:14', '2018-01-15 16:37:24', 1),
(81, 2, '2018-01-15 16:37:31', '2018-01-15 23:40:31', 1),
(82, 1, '2018-01-15 16:37:39', '2018-01-15 23:40:38', 1);

-- --------------------------------------------------------

--
-- Tábla szerkezet ehhez a táblához `users`
--

CREATE TABLE `users` (
  `user_id` int(11) NOT NULL,
  `first_name` varchar(40) CHARACTER SET utf8 COLLATE utf8_hungarian_ci NOT NULL,
  `last_name` varchar(40) CHARACTER SET utf8 COLLATE utf8_hungarian_ci NOT NULL,
  `card_id` varchar(11) CHARACTER SET utf8 COLLATE utf8_hungarian_ci NOT NULL,
  `is_clocked_in` tinyint(1) NOT NULL,
  `total_hours` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- A tábla adatainak kiíratása `users`
--

INSERT INTO `users` (`user_id`, `first_name`, `last_name`, `card_id`, `is_clocked_in`, `total_hours`) VALUES
(1, 'Kek', 'Kulcstarto', '45 9E 8E B0', 0, 560),
(2, 'Feher', 'Kartya', '4A 8A B8 A9', 0, 560);

--
-- Indexek a kiírt táblákhoz
--

--
-- A tábla indexei `events`
--
ALTER TABLE `events`
  ADD PRIMARY KEY (`clock_id`);

--
-- A tábla indexei `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`user_id`),
  ADD UNIQUE KEY `card_id` (`card_id`);

--
-- A kiírt táblák AUTO_INCREMENT értéke
--

--
-- AUTO_INCREMENT a táblához `events`
--
ALTER TABLE `events`
  MODIFY `clock_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=83;
--
-- AUTO_INCREMENT a táblához `users`
--
ALTER TABLE `users`
  MODIFY `user_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
