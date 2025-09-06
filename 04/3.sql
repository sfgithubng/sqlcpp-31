--Num artists per genre
SELECT COUNT(idartist) as numartists, name as genrename FROM genres, artistgenres WHERE (genres.idgenre = artistgenres.idgenre) GROUP BY artistgenres.idgenre, name;

--Num songs in 2019-2020 albums
SELECT COUNT(idsong) as numsongs, name as albumname FROM albums, albumsongs WHERE (albums.releasedate BETWEEN '2019-01-01' AND '2020-12-31') AND (albums.idalbum = albumsongs.idalbum) GROUP BY albums.idalbum;

--Average track lengs for each album
SELECT AVG(duration), albums.name as albumname FROM albums, albumsongs, songs WHERE (albums.idalbum = albumsongs.idalbum) AND (songs.idsong = albumsongs.idsong) GROUP BY albums.idalbum;

--Artists with no albums in 2020
SELECT albums.name FROM albums JOIN albumartists ON albumartists.idalbum = albums.idalbum JOIN artists ON albumartists.idartist = artists.idartist WHERE (albums.releasedate NOT BETWEEN '2020-01-01' AND '2020-12-31');

--Compilation albums with Bobby
SELECT compilationalbums.name FROM compilationalbums JOIN compilationalbumartists ON compilationalbumartists.idcompilationalbum = compilationalbums.idcompilationalbum JOIN artists ON compilationalbumartists.idartist = artists.idartist WHERE (artists.fullname = 'Bobby Talent');
