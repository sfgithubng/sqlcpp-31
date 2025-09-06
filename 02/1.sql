CREATE TABLE Genres (
  IdGenre int PRIMARY KEY CHECK (IdGenre > 0),
  Name varchar(255) NOT NULL
);

CREATE TABLE Artists (
  IdArtist int PRIMARY KEY CHECK (IdArtist > 0),
  FullName varchar(255) NOT NULL,
  NickName varchar(255) NOT NULL
);

CREATE TABLE Albums (
  IdAlbum int PRIMARY KEY CHECK (IdAlbum > 0),
  Name varchar(255) NOT NULL,
  ReleaseDate timestamp with time zone DEFAULT now() NOT NULL
);

CREATE TABLE CompilationAlbums (
  IdCompilationAlbum int PRIMARY KEY CHECK (IdCompilationAlbum > 0),
  Name varchar(255) NOT NULL,
  ReleaseDate timestamp with time zone DEFAULT now() NOT NULL
);

CREATE TABLE Songs (
  IdSong int PRIMARY KEY CHECK (IdSong > 0),
  Name varchar(255) NOT NULL,
  Duration int NOT NULL CHECK (Duration > 0)
);

CREATE TABLE ArtistGenres (
  IdArtist int,
  IdGenre int,
  PRIMARY KEY (IdArtist, IdGenre),
  FOREIGN KEY (IdArtist) REFERENCES Artists(IdArtist),
  FOREIGN KEY (IdGenre) REFERENCES Genres(IdGenre)
);

CREATE TABLE AlbumArtists (
  IdAlbum int,
  IdArtist int,
  PRIMARY KEY (IdAlbum, IdArtist),
  FOREIGN KEY (IdAlbum) REFERENCES Albums(IdAlbum),
  FOREIGN KEY (IdArtist) REFERENCES Artists(IdArtist)
);

CREATE TABLE AlbumSongs (
  IdAlbum int,
  IdSong int,
  PRIMARY KEY (IdAlbum, IdSong),
  FOREIGN KEY (IdAlbum) REFERENCES Albums(IdAlbum),
  FOREIGN KEY (IdSong) REFERENCES Songs(IdSong)
);

CREATE TABLE CompilationAlbumSongs (
  IdCompilationAlbum int,
  IdSong int,
  PRIMARY KEY (IdCompilationAlbum, IdSong),
  FOREIGN KEY (IdCompilationAlbum) REFERENCES CompilationAlbums(IdCompilationAlbum),
  FOREIGN KEY (IdSong) REFERENCES Songs(IdSong)
);

CREATE TABLE CompilationAlbumArtists (
  IdCompilationAlbum int,
  IdArtist int,
  PRIMARY KEY (IdCompilationAlbum, IdArtist),
  FOREIGN KEY (IdCompilationAlbum) REFERENCES CompilationAlbums(IdCompilationAlbum),
  FOREIGN KEY (IdArtist) REFERENCES Artists(IdArtist)
);
