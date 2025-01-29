-- Keep a log of any SQL queries you execute as you solve the mystery.
SELECT description FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28 AND street = 'Humphrey Street';
--collecting data related to the crime (description and more details)
Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
| Littering took place at 16:36. No known witnesses.
SELECT id FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28 AND street = 'Humphrey Street';
--looking for the bakery's ID
SELECT transcript FROM interviews WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE '%bakery%';
--looking for the three transcripts related to the crime
SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25;
 5P2BI95       |
| 94KL13X       |
| 6P58WS2       |
| 4328GD8       |
| G412CB7       |
| L93JTIZ       |
| 322W7JE       |
| 0NTHK55
 SELECT account_number, id FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street';
--looking for the account_number and id of the thief according to the information provided in the transcripts
SELECT caller, receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;
-- giving the number of the caller and the receiver
SELECT id, name FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25);
--this outputs four ids and names of potential suspects (thief)
  id   |  name  |
+--------+--------+
| 398010 | Sofia  |
| 514354 | Diana  |
| 560886 | Kelsey |
| 686048 | Bruce
SELECT id, name FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE year = 2021 AND
month = 7 AND day = 28 AND duration < 60);
-- prints out the possible accomplices
SELECT * FROM airports WHERE city = 'Fiftyville';
-- prints out the information about Fiftyville's airport
 SELECT * FROM flights WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id IN (SELECT id FRO
M airports WHERE city = 'Fiftyville') ORDER BY hour, minute LIMIT 1;
--this query gives the exact flight that the thief will be taking the id of the airport is equal to 4 and the id of the flight is equal to 36
SELECT city FROM airports WHERE id = 4;
--the city is New York City
SELECT id,name FROM people JOIN passengers ON people.passport_number = passengers.passport_number WHERE flight_id = 36 AND phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25);
--this restricts the number of suspects to three instead of four
SELECT id, name FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id JOIN passengers ON people.passport_number = passengers.passport_number WHERE flight_id = 36 AND phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25);
--Bruce is the thief
SELECT phone_number,name FROM people WHERE id = 686048;
--prints out the phone_number of the thief who is the caller
SELECT id, name FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller = '(367) 555-5533');
--prints out the accomplice : Robin with an ID = 864400
