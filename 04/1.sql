INSERT INTO artists (idartist, fullname, nickname) VALUES (1, 'Bobby Talent', 'Bobby');
INSERT INTO artists (idartist, fullname, nickname) VALUES (2, 'John Smith', 'Johny');
INSERT INTO artists (idartist, fullname, nickname) VALUES (3, 'Michael Stepaway', 'Mike');
INSERT INTO artists (idartist, fullname, nickname) VALUES (4, 'Johann Drallematos', 'Rally');

INSERT INTO genres (idgenre, name) VALUES (1, 'Classical');
INSERT INTO genres (idgenre, name) VALUES (2, 'Rock');
INSERT INTO genres (idgenre, name) VALUES (3, 'Jazz');

INSERT INTO albums (idalbum, name, releasedate) VALUES (1, 'The Machine', '2020-01-07');
INSERT INTO albums (idalbum, name, releasedate) VALUES (2, 'Ordinary', '2022-05-15');
INSERT INTO albums (idalbum, name, releasedate) VALUES (3, 'Not less until too much', '2018-10-10');

INSERT INTO songs (idsong, name, duration) VALUES (1, 'Homeinfo', 84);
INSERT INTO songs (idsong, name, duration) VALUES (2, 'Resulting behing', 54);
INSERT INTO songs (idsong, name, duration) VALUES (3, 'Narrow escape', 67);
INSERT INTO songs (idsong, name, duration) VALUES (4, 'Creepy walnut', 142);
INSERT INTO songs (idsong, name, duration) VALUES (5, 'Common coutnertop', 95);
INSERT INTO songs (idsong, name, duration) VALUES (6, 'Surprisingly notorious', 83);
INSERT INTO songs (idsong, name, duration) VALUES (7, 'Devastating writeoff', 226);
INSERT INTO songs (idsong, name, duration) VALUES (8, 'Whereever', 172);

INSERT INTO compilationalbums (idcompilationalbum, name, releasedate) VALUES (1, 'The best of Bobby Vol.1', '2023-12-08');
INSERT INTO compilationalbums (idcompilationalbum, name, releasedate) VALUES (2, 'Mike Rocks', '2021-11-15');
INSERT INTO compilationalbums (idcompilationalbum, name, releasedate) VALUES (3, 'Swipe by Rally', '2023-04-10');
INSERT INTO compilationalbums (idcompilationalbum, name, releasedate) VALUES (4, 'The depth of Johny', '2020-05-23');

INSERT INTO albumartists (idalbum, idartist) VALUES (1, 1);
INSERT INTO albumartists (idalbum, idartist) VALUES (2, 2);
INSERT INTO albumartists (idalbum, idartist) VALUES (3, 3);

INSERT INTO albumsongs (idalbum, idsong) VALUES (1, 1);
INSERT INTO albumsongs (idalbum, idsong) VALUES (2, 2);
INSERT INTO albumsongs (idalbum, idsong) VALUES (3, 3);

INSERT INTO artistgenres (idartist, idgenre) VALUES (1, 2);
INSERT INTO artistgenres (idartist, idgenre) VALUES (2, 2);
INSERT INTO artistgenres (idartist, idgenre) VALUES (3, 1);
INSERT INTO artistgenres (idartist, idgenre) VALUES (3, 3);
INSERT INTO artistgenres (idartist, idgenre) VALUES (4, 2);
INSERT INTO artistgenres (idartist, idgenre) VALUES (4, 3);

INSERT INTO compilationalbumsongs (idcompilationalbum, idsong) VALUES (1, 1); 
INSERT INTO compilationalbumsongs (idcompilationalbum, idsong) VALUES (1, 5);
INSERT INTO compilationalbumsongs (idcompilationalbum, idsong) VALUES (2, 2); 
INSERT INTO compilationalbumsongs (idcompilationalbum, idsong) VALUES (2, 6);
INSERT INTO compilationalbumsongs (idcompilationalbum, idsong) VALUES (3, 3); 
INSERT INTO compilationalbumsongs (idcompilationalbum, idsong) VALUES (3, 7);
INSERT INTO compilationalbumsongs (idcompilationalbum, idsong) VALUES (4, 4); 
INSERT INTO compilationalbumsongs (idcompilationalbum, idsong) VALUES (4, 8);
