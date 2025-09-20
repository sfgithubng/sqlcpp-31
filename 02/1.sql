CREATE TABLE Genres (
  IdGenre int PRIMARY KEY CHECK (IdGenre > 0),
  Name varchar(255) NOT NULL
);

CREATE TABLE Artists (
  IdArtist int PRIMARY KEY CHECK (IdArtist > 0),
  FullName varchar(255) NOT NULL,
  NickName varchar(255) NOT NULL
);

CREATE TABLE ArtistGenres (
  IdArtist int,
  IdGenre int,
  PRIMARY KEY (IdArtist, IdGenre),
  FOREIGN KEY (IdArtist) REFERENCES Artists(IdArtist),
  FOREIGN KEY (IdGenre) REFERENCES Genres(IdGenre)
);

CREATE TABLE Albums (
  IdAlbum int PRIMARY KEY CHECK (IdAlbum > 0),
  Name varchar(255) NOT NULL,
  ReleaseDate timestamp with time zone DEFAULT now() NOT NULL
);

CREATE TABLE AlbumArtists (
  IdAlbum int,
  IdArtist int,
  PRIMARY KEY (IdAlbum, IdArtist),
  FOREIGN KEY (IdAlbum) REFERENCES Albums(IdAlbum),
  FOREIGN KEY (IdArtist) REFERENCES Artists(IdArtist)
);

CREATE TABLE Songs (
  IdSong int PRIMARY KEY CHECK (IdSong > 0),
  IdAlbum int,
  Name varchar(255) NOT NULL,
  Duration int NOT NULL CHECK (Duration > 0),
  FOREIGN KEY (IdAlbum) REFERENCES Albums(IdAlbum)
);

CREATE TABLE CompilationAlbums (
  IdCompilationAlbum int PRIMARY KEY CHECK (IdCompilationAlbum > 0),
  Name varchar(255) NOT NULL,
  ReleaseDate timestamp with time zone DEFAULT now() NOT NULL
);

CREATE TABLE CompilationAlbumSongs (
  IdCompilationAlbum int,
  IdSong int,
  PRIMARY KEY (IdCompilationAlbum, IdSong),
  FOREIGN KEY (IdCompilationAlbum) REFERENCES CompilationAlbums(IdCompilationAlbum),
  FOREIGN KEY (IdSong) REFERENCES Songs(IdSong)
);