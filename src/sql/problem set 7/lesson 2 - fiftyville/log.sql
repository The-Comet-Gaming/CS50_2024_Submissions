-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Firstly at the start of each session or codespace reset.
-- I always navigate to the correct folder where the fiftyville files are located
cd sql/fifftyville/

-- I run this command each time I'm opening the database
sqlite3 fiftyville.db


-- I started by getting a lay of the land so to speak by using .schema to
-- retrieve the commands used to create the table and get a clear picture
-- of how the tables connect and interact with eachother
.schema

-- As per the hint in the Fiftyville problem description and as logic
-- would naturally suggest, I need to access the table of crime scene reports
-- filtering out all entries that do not match the time, date and location of
-- the crime in question.
-- If I wasn't being lazy, I would have only selected 'id' and 'description'
-- from 'crime_scene_reports but I wasn't worried about the additional data
-- being retrieved and decided to save myself the additional keystrokes.
SELECT *
FROM crime_scene_reports
WHERE year = 2023 AND month = 7 AND day = 28 AND street = 'Humphrey Street';
-- The table retrieved is as follows:
/*
+-----+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| id  | year | month | day |     street      |                                                                                                       description                                                                                                        |
+-----+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 295 | 2023 | 7     | 28  | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
| 297 | 2023 | 7     | 28  | Humphrey Street | Littering took place at 16:36. No known witnesses.                                                                                                                                                                       |
+-----+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- Only the first entry is important to us that being:
+-----+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 295 | 2023 | 7     | 28  | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
+-----+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- The notable details we have found are the time: '10:15am', location/address of the street: 'bakery', and that there were 3 witnesses and subsequently 3 interviews, all of which mention the 'bakery'.*/

-- Now that we have some more details and knowing that we have interviews from witnesses of the crime.
-- We need to access the 'interviews' table and filter out rows that do not contain our pertinent information.
SELECT id, name, transcript
FROM interviews
WHERE year = 2023 AND month = 7 AND day = 28 AND transcript LIKE '%bakery%';
-- The table retrieved is as follows:
/*
+-----+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| id  |  name   |                                                                                                                                                     transcript                                                                                                                                                      |
+-----+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 161 | Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| 162 | Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| 163 | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
+-----+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- Wow, that's a long table. I'm glad that I chose to specify what columns I was retrieving this time or else that table would have been bigger.*/
-- These were some pretty good witnesses though, we now have a pretty clear picture of how things transpired and some threads to pull on:
-- Like the thief escaping in a car from the bakery parking lot and what time frame this happened in.
-- The thief being recognised at the ATM located on 'Leggett Street' before the theft.
-- That the thief made a short, less than a minute long phone call where the thief mentions their plan to take the earliest flight out of Fiftyville the next morning.
-- And that the thief asked the person on the other end of the call to make the flight ticket purchase for them.

-- Let's start pulling on the thread of the getaway car first.
-- Firstly I want to visually see what the table looks like and what data is contained within the columns.
SELECT *
FROM bakery_security_logs
LIMIT 10;
-- The table retrieved is as follows:
/*
+----+------+-------+-----+------+--------+----------+---------------+
| id | year | month | day | hour | minute | activity | license_plate |
+----+------+-------+-----+------+--------+----------+---------------+
| 1  | 2023 | 7     | 25  | 7    | 1      | entrance | 0R0FW39       |
| 2  | 2023 | 7     | 25  | 7    | 31     | entrance | 97O6H62       |
| 3  | 2023 | 7     | 25  | 7    | 58     | entrance | 5840J5X       |
| 4  | 2023 | 7     | 25  | 8    | 5      | entrance | JUP02H1       |
| 5  | 2023 | 7     | 25  | 8    | 6      | entrance | TWA51P1       |
| 6  | 2023 | 7     | 25  | 8    | 11     | entrance | 50U175Y       |
| 7  | 2023 | 7     | 25  | 8    | 22     | entrance | HW0BF87       |
| 8  | 2023 | 7     | 25  | 8    | 25     | entrance | 5209A97       |
| 9  | 2023 | 7     | 25  | 8    | 28     | entrance | C3S4W87       |
| 10 | 2023 | 7     | 25  | 8    | 30     | entrance | B49OR81       |
+----+------+-------+-----+------+--------+----------+---------------+
-- Here I can see how the columns are utelised.*/
-- Specifically the hour and minute columns which are estential to narrowing down our list of possible getaway cars.

-- With that in mind. I plan to use the following query to retrieve security log information during 10.15am to 10.25am
-- and link the license_plate to the table of people to retrieve the id, name, phone and number along with other relevant information.
SELECT b.hour, b.minute, b.activity, b.license_plate, p.*
FROM bakery_security_logs AS b
JOIN people AS p ON b.license_plate = p.license_plate
WHERE year = 2023 AND month = 7 AND day = 28 AND b.hour = 10 AND minute >= 15 AND minute <= 25;
-- The table retrieved is as follows:
/*
+------+--------+----------+---------------+--------+---------+----------------+-----------------+---------------+
| hour | minute | activity | license_plate |   id   |  name   |  phone_number  | passport_number | license_plate |
+------+--------+----------+---------------+--------+---------+----------------+-----------------+---------------+
| 10   | 16     | exit     | 5P2BI95       | 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
| 10   | 18     | exit     | 94KL13X       | 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
| 10   | 18     | exit     | 6P58WS2       | 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
| 10   | 19     | exit     | 4328GD8       | 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
| 10   | 20     | exit     | G412CB7       | 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
| 10   | 21     | exit     | L93JTIZ       | 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
| 10   | 23     | exit     | 322W7JE       | 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
| 10   | 23     | exit     | 0NTHK55       | 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
+------+--------+----------+---------------+--------+---------+----------------+-----------------+---------------+
*/
-- Here I have 8 individual vehicles, their number plates, and the people who's name they are registered under.
-- Since we can't make any assumptions about who or are how these people are involved in with the crime.
-- We will keep a note of these people for and continue on pulling at other threads.

-- Let's move onto the culprit's alledged appearence at the ATM on Leggett Street.
SELECT p.*, a.account_number, a.transaction_type, a.amount, a.atm_location
FROM atm_transactions AS a
JOIN bank_accounts AS b ON a.account_number = b.account_number
JOIN people AS p ON b.person_id = p.id
WHERE a.year = 2023 AND a.month = 7 AND a.day = 28 AND a.atm_location = 'Leggett Street';
-- The table retrieved is as follows:
/*
+--------+---------+----------------+-----------------+---------------+----------------+------------------+--------+----------------+
|   id   |  name   |  phone_number  | passport_number | license_plate | account_number | transaction_type | amount |  atm_location  |
+--------+---------+----------------+-----------------+---------------+----------------+------------------+--------+----------------+
| 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | withdraw         | 50     | Leggett Street |
| 948985 | Kaelyn  | (098) 555-1164 | 8304650265      | I449449       | 86363979       | deposit          | 10     | Leggett Street |
| 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       | 26013199       | withdraw         | 35     | Leggett Street |
| 458378 | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       | 16153065       | withdraw         | 80     | Leggett Street |
| 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       | 28296815       | withdraw         | 20     | Leggett Street |
| 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       | 25506511       | withdraw         | 20     | Leggett Street |
| 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       | 28500762       | withdraw         | 48     | Leggett Street |
| 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       | 76054385       | withdraw         | 60     | Leggett Street |
| 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       | 81061156       | withdraw         | 30     | Leggett Street |
+--------+---------+----------------+-----------------+---------------+----------------+------------------+--------+----------------+
*/
-- Now so far we have two people who were present at both locations on the day of the crime. That being:
/*
+--------+---------+----------------+-----------------+---------------+
|   id   |  name   |  phone_number  | passport_number | license_plate |
+--------+---------+----------------+-----------------+---------------+
| 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
| 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
+--------+---------+----------------+-----------------+---------------+
-- Let's see what the search of the phone calls table brings up.*/
SELECT p.id, p.name, pc.*
FROM phone_calls AS pc
JOIN people AS p ON pc.caller = p.phone_number
WHERE pc.year = 2023 AND pc.month = 7 AND pc.day = 28 AND pc.duration <= 60;
/*
+--------+---------+-----+----------------+----------------+------+-------+-----+----------+
|   id   |  name   | id  |     caller     |    receiver    | year | month | day | duration |
+--------+---------+-----+----------------+----------------+------+-------+-----+----------+
| 398010 | Sofia   | 221 | (130) 555-0289 | (996) 555-8899 | 2023 | 7     | 28  | 51       |
| 560886 | Kelsey  | 224 | (499) 555-9472 | (892) 555-8872 | 2023 | 7     | 28  | 36       |
| 686048 | Bruce   | 233 | (367) 555-5533 | (375) 555-8161 | 2023 | 7     | 28  | 45       |
| 561160 | Kathryn | 234 | (609) 555-5876 | (389) 555-5198 | 2023 | 7     | 28  | 60       |
| 560886 | Kelsey  | 251 | (499) 555-9472 | (717) 555-1342 | 2023 | 7     | 28  | 50       |
| 449774 | Taylor  | 254 | (286) 555-6063 | (676) 555-6554 | 2023 | 7     | 28  | 43       |
| 514354 | Diana   | 255 | (770) 555-1861 | (725) 555-3243 | 2023 | 7     | 28  | 49       |
| 907148 | Carina  | 261 | (031) 555-6622 | (910) 555-3251 | 2023 | 7     | 28  | 38       |
| 395717 | Kenny   | 279 | (826) 555-1652 | (066) 555-9701 | 2023 | 7     | 28  | 55       |
| 438727 | Benista | 281 | (338) 555-6650 | (704) 555-2131 | 2023 | 7     | 28  | 54       |
+--------+---------+-----+----------------+----------------+------+-------+-----+----------+
*/
-- We can make that a little bit better though.

-- So I may have gone a little overboard compared to what I had written before.
SELECT p1.id AS suspect_id, p1.name AS suspect_name, pc.caller AS suspect_phone,
p2.id AS receiver_id, p2.name AS receiver_name, pc.receiver, pc.duration
FROM people AS p1
JOIN phone_calls AS pc ON p1.phone_number = pc.caller
JOIN people AS p2 ON pc.receiver = p2.phone_number
JOIN bakery_security_logs AS bakery ON p1.license_plate = bakery.license_plate
JOIN bank_accounts AS bank ON p1.id = bank.person_id
JOIN atm_transactions AS a ON bank.account_number = a.account_number
WHERE bakery.year = 2023 AND bakery.month = 7 AND bakery.day = 28
AND bakery.hour = 10 AND bakery.minute >= 15 AND bakery.minute <= 25
AND pc.year = 2023 AND pc.month = 7 AND pc.day = 28 AND pc.duration <= 60
AND a.year = 2023 AND a.month = 7 AND a.day = 28 AND a.atm_location = 'Leggett Street';
-- Table retrieved as follows:
/*
+------------+--------------+----------------+-------------+---------------+----------------+----------+
| suspect_id | suspect_name | suspect_phone  | receiver_id | receiver_name |    receiver    | duration |
+------------+--------------+----------------+-------------+---------------+----------------+----------+
| 686048     | Bruce        | (367) 555-5533 | 864400      | Robin         | (375) 555-8161 | 45       |
| 514354     | Diana        | (770) 555-1861 | 847116      | Philip        | (725) 555-3243 | 49       |
+------------+--------------+----------------+-------------+---------------+----------------+----------+
*/
-- But by taking all of the previous pieces of information I had gathered more or less
-- independantly of eachother and combining them into a single query I was able to
-- narrow down the possible suspects to a Bruce or Diana.
-- Interestingly enough I hadn't even noticed Diana as a possible suspect during
-- the independant queries but upon going back through these notes I found that she was
-- indeed present at all of the same locations as the thief within the same estimated time frames.

-- Next I need to check the flights out of Fiftyville that were scheduled for the next morning.
-- I went through several iterations of this query until I arrived at a result that I felt was satisfactory
-- in the evidence that it provided.

-- #1
SELECT p1.id AS suspect_id, p1.name AS suspect_name, p3.seat AS suspect_seat, p3.flight_id AS flight
FROM people AS p1
JOIN phone_calls AS pc ON p1.phone_number = pc.caller
JOIN people AS p2 ON pc.receiver = p2.phone_number
JOIN bakery_security_logs AS bakery ON p1.license_plate = bakery.license_plate
JOIN bank_accounts AS bank ON p1.id = bank.person_id
JOIN atm_transactions AS a ON bank.account_number = a.account_number
JOIN passengers AS p3 ON p1.passport_number = p3.passport_number
WHERE bakery.year = 2023 AND bakery.month = 7 AND bakery.day = 28
AND bakery.hour = 10 AND bakery.minute >= 15 AND bakery.minute <= 25
AND pc.year = 2023 AND pc.month = 7 AND pc.day = 28 AND pc.duration <= 60
AND a.year = 2023 AND a.month = 7 AND a.day = 28 AND a.atm_location = 'Leggett Street';

-- #2
SELECT p1.id AS suspect_id, p1.name AS suspect_name, p3.seat AS suspect_seat, p3.flight_id AS flight
FROM people AS p1
JOIN phone_calls AS pc ON p1.phone_number = pc.caller
JOIN people AS p2 ON pc.receiver = p2.phone_number
JOIN bakery_security_logs AS bakery ON p1.license_plate = bakery.license_plate
JOIN bank_accounts AS bank ON p1.id = bank.person_id
JOIN atm_transactions AS a ON bank.account_number = a.account_number
JOIN passengers AS p3 ON p1.passport_number = p3.passport_number
JOIN flights AS f ON p3.flight_id = f.id
WHERE bakery.year = 2023 AND bakery.month = 7 AND bakery.day = 28
AND bakery.hour = 10 AND bakery.minute >= 15 AND bakery.minute <= 25
AND pc.year = 2023 AND pc.month = 7 AND pc.day = 28 AND pc.duration <= 60
AND a.year = 2023 AND a.month = 7 AND a.day = 28 AND a.atm_location = 'Leggett Street'
AND f.year = 2023 AND f.month = 7 AND f.day = 29;

-- I used the following query to get a visual aid for what data was held in the table:
SELECT *
FROM flights;

-- #3
SELECT p1.id AS suspect_id, p1.name AS suspect_name, p3.*, f.*
FROM people AS p1
JOIN phone_calls AS pc ON p1.phone_number = pc.caller
JOIN people AS p2 ON pc.receiver = p2.phone_number
JOIN bakery_security_logs AS bakery ON p1.license_plate = bakery.license_plate
JOIN bank_accounts AS bank ON p1.id = bank.person_id
JOIN atm_transactions AS a ON bank.account_number = a.account_number
JOIN passengers AS p3 ON p1.passport_number = p3.passport_number
JOIN flights AS f ON p3.flight_id = f.id
WHERE bakery.year = 2023 AND bakery.month = 7 AND bakery.day = 28
AND bakery.hour = 10 AND bakery.minute >= 15 AND bakery.minute <= 25
AND pc.year = 2023 AND pc.month = 7 AND pc.day = 28 AND pc.duration <= 60
AND a.year = 2023 AND a.month = 7 AND a.day = 28 AND a.atm_location = 'Leggett Street'
AND f.year = 2023 AND f.month = 7 AND f.day = 29;
-- This query produced the following table:
/*
+------------+--------------+-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
| suspect_id | suspect_name | flight_id | passport_number | seat | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+------------+--------------+-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
| 686048     | Bruce        | 36        | 5773159633      | 4A   | 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
| 514354     | Diana        | 18        | 3592750733      | 4C   | 18 | 8                 | 6                      | 2023 | 7     | 29  | 16   | 0      |
+------------+--------------+-----------+-----------------+------+----+-------------------+------------------------+------+-------+-----+------+--------+
*/
-- Bruce edges out Diana as prime suspect due to his flight leaving earlier.

-- Let's get a bit more clarity on one or two details.
SELECT p1.id AS suspect_id, p1.name AS suspect_name, p1.passport_number AS suspect_passport, p3.flight_id, p3.seat AS seat_number,
ap1.city AS origin_airport, ap2.city AS dest_airport, f.year, f.month, f.day, f.hour, f.minute
FROM people AS p1
JOIN phone_calls AS pc ON p1.phone_number = pc.caller
JOIN people AS p2 ON pc.receiver = p2.phone_number
JOIN bakery_security_logs AS bakery ON p1.license_plate = bakery.license_plate
JOIN bank_accounts AS bank ON p1.id = bank.person_id
JOIN atm_transactions AS a ON bank.account_number = a.account_number
JOIN passengers AS p3 ON p1.passport_number = p3.passport_number
JOIN flights AS f ON p3.flight_id = f.id
JOIN airports AS ap1 ON f.origin_airport_id = ap1.id
JOIN airports AS ap2 ON f.destination_airport_id = ap2.id
WHERE bakery.year = 2023 AND bakery.month = 7 AND bakery.day = 28
AND bakery.hour = 10 AND bakery.minute >= 15 AND bakery.minute <= 25
AND pc.year = 2023 AND pc.month = 7 AND pc.day = 28 AND pc.duration <= 60
AND a.year = 2023 AND a.month = 7 AND a.day = 28 AND a.atm_location = 'Leggett Street'
AND f.year = 2023 AND f.month = 7 AND f.day = 29;
/*
+------------+--------------+------------------+-----------+-------------+----------------+---------------+------+-------+-----+------+--------+
| suspect_id | suspect_name | suspect_passport | flight_id | seat_number | origin_airport | dest_airport  | year | month | day | hour | minute |
+------------+--------------+------------------+-----------+-------------+----------------+---------------+------+-------+-----+------+--------+
| 514354     | Diana        | 3592750733       | 18        | 4C          | Fiftyville     | Boston        | 2023 | 7     | 29  | 16   | 0      |
| 686048     | Bruce        | 5773159633       | 36        | 4A          | Fiftyville     | New York City | 2023 | 7     | 29  | 8    | 20     |
+------------+--------------+------------------+-----------+-------------+----------------+---------------+------+-------+-----+------+--------+
*/
-- As we can see from the table above, Bruce is the prime suspect over Diana due to their flight's departure time.
-- Before we can pin down Bruce however we need to check who paid for the respect suspect's flights.
-- As if the testimony fron Raymond is correct, it wasn't the suspect themselves who booked/paid for the flight, it was their accomplice.

-- At this point I went to see if I could find any transactions that would tie the accomplice to the flight of the prime suspects,
-- but I don't think this mystery accounted for that angle of investigation.
SELECT *
FROM atm_transactions AS atm
JOIN bank_accounts AS bank ON atm.account_number = bank.account_number
JOIN people AS p ON bank.person_id = p.id
WHERE atm.year = 2023 AND atm.month = 7 AND atm.day = 28 AND p.name = 'Robin' OR p.name = 'Philip';


-- TLDR: Bruce is the culprit!, his accomplice is Robin, and he escaped to New York City!
