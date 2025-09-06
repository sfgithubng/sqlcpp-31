--Name and length of the longest song
SELECT name, duration FROM songs WHERE duration IN (SELECT MAX(duration) FROM songs);

--Names of songs which are not less than 210s
SELECT name FROM songs WHERE duration >= 210;

--Compilation album names released from 2018 to 2020
SELECT name FROM compilationalbums WHERE releasedate BETWEEN '2018-01-01' AND '2020-12-31';

--Artists who haven't released albums in 2020
SELECT fullname, nickname FROM artists WHERE fullname NOT LIKE '% %';

--Songs with name matching "my"
SELECT name FROM songs WHERE name LIKE '%my%';
