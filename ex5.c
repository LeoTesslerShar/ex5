/******************
Name: Leo Tessler Shar
ID: 323919274
Assignment: ex5
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 16

typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
} Playlist;

void deleteSong(Playlist* playlist, Song **songs, int dIndex);
void deletePlaylist(Playlist* playlist, int dIndex, int pNum);
void playSong(Playlist *playlist, int pIndex, int sIndex);
void freeSong(Song *song);
void freePlaylists(Playlist* playlist, int* pNum);
void printPlaylistsMenu(Playlist *playlists, int counter);
void printMainMenu();
void addPlaylist(Playlist **playlists, int index);
void printSinglePlaylistMenu();
void showPlaylist(Playlist *playlists, int index);
void addSong(Playlist **playlist, int pIndex, int sIndex);
void playAllSongs(Playlist *playlist, int pIndex);
void sortYear(Song **songs, int sNum);
void sortStreamsAscend(Song **songs, int sNum);
void sortStreamsDescend(Song **songs, int sNum);
void sortAlpha(Song **songs, int sNum);

int main() {
    int task, pCounter = 0;
    Playlist* playlists = NULL;
    do {
        printMainMenu();
        if (scanf("%d", &task)) {
            switch (task) {
                case 4:
                    freePlaylists(playlists, &pCounter);
                    free(playlists);
                    printf("Goodbye!\n");
                    break;
                case 1:
                    int pNum, pTask;
                    do {
                        // Prints all the playlists
                        printPlaylistsMenu(playlists, pCounter);
                        if (scanf("%d", &pNum) && pNum != pCounter + 1) {
                            // Prints a selected playlist's menu
                            printf("playlist %s:\n", playlists[pNum - 1].name);
                            do {
                                printSinglePlaylistMenu();
                                if (scanf("%d", &pTask)) {
                                    switch (pTask) {
                                        case 1:
                                            showPlaylist(playlists, pNum - 1);
                                            int songChoice;
                                            do {
                                                printf("choose a song to play, or 0 to quit:\n");
                                                scanf("%d", &songChoice);
                                                 /*If the playlist is empty go back to the menu with any input.
                                                   If choice is bigger than the number of the songs
                                                   or smaller than 0, go back to the menu*/
                                                if (playlists[pNum - 1].songsNum == 0 ||
                                                    songChoice > playlists[pNum - 1].songsNum || songChoice <= 0)
                                                    break;
                                                playSong(playlists, pNum - 1, songChoice - 1);
                                            } while (songChoice <= playlists[pNum - 1].songsNum && songChoice > 0);
                                            break;
                                        case 2:
                                            if (playlists[pNum - 1].songs == NULL) {
                                                playlists[pNum - 1].songs = malloc(sizeof(Song*));
                                            }
                                            // Expanding the memory of Songs by 1
                                            playlists[pNum - 1].songs =
                                                realloc(playlists[pNum - 1].songs,sizeof(Song*) *
                                                    (playlists[pNum - 1].songsNum + 1));
                                            if (playlists[pNum - 1].songs == NULL)
                                                exit(1);
                                            // Setting a new song
                                            Song* newSong = malloc(sizeof(Song));
                                            if (newSong == NULL)
                                                exit(1);
                                            newSong->title = NULL;
                                            newSong->artist = NULL;
                                            newSong->lyrics = NULL;
                                            newSong->streams = 0;
                                            playlists[pNum - 1].songs[playlists[pNum - 1].songsNum] = newSong;
                                            addSong(&playlists, pNum - 1, playlists[pNum-1].songsNum);
                                            playlists[pNum - 1].songsNum++;
                                            break;
                                        case 3:
                                            showPlaylist(playlists, pNum - 1);
                                            printf("choose a song to delete, or 0 to quit:\n");
                                            int dSong;
                                            scanf("%d", &dSong);
                                            if(dSong == 0)
                                                break;
                                            deleteSong(&playlists[pNum - 1], playlists[pNum - 1].songs,
                                                 dSong - 1);
                                            if (playlists[pNum - 1].songsNum > 0) {
                                                playlists[pNum - 1].songs = realloc(playlists[pNum - 1].songs,
                                                    sizeof(Song*) * playlists[pNum - 1].songsNum);
                                            }
                                            break;
                                        case 4:
                                            printf("choose:\n"
                                                   "1. sort by year\n"
                                                   "2. sort by streams - ascending order\n"
                                                   "3. sort by streams - descending order \n"
                                                   "4. sort alphabetically\n");
                                            int choice;
                                            scanf("%d", &choice);
                                            if (choice < 1 || choice > 4)
                                                sortAlpha(playlists[pNum - 1].songs, playlists[pNum - 1].songsNum);
                                            else {
                                                if (choice == 1)
                                                    sortYear(playlists[pNum - 1].songs, playlists[pNum - 1].songsNum);
                                                if (choice == 2)
                                                    sortStreamsAscend(playlists[pNum - 1].songs,
                                                        playlists[pNum - 1].songsNum);
                                                if (choice == 3)
                                                    sortStreamsDescend(playlists[pNum - 1].songs,
                                                        playlists[pNum - 1].songsNum);
                                                if (choice == 4)
                                                    sortAlpha(playlists[pNum - 1].songs,
                                                        playlists[pNum - 1].songsNum);
                                            }
                                            break;
                                        case 5:
                                            playAllSongs(playlists, pNum - 1);
                                            break;
                                        case 6:
                                            break;
                                        default:
                                            printf("Invalid input");
                                            break;
                                    }
                                }
                                else
                                    printf("Invalid input\n");
                            } while (pTask != 6);
                        }
                        else if (pNum != pCounter + 1){
                            printf("Invalid input\n");
                        }
                    } while (pNum != pCounter + 1);
                    break;
                case 2:
                    if (playlists == NULL) {
                        playlists = (Playlist*) malloc(sizeof(Playlist));
                        if (playlists == NULL)
                            exit(1);
                    }
                    else {
                        playlists = (Playlist*) realloc(playlists, sizeof(Playlist) * (pCounter + 1));
                        if (playlists == NULL)
                            exit(1);
                    }
                    addPlaylist(&playlists, pCounter);
                    pCounter++;
                    break;
                case 3:
                    printPlaylistsMenu(playlists, pCounter);
                    int delIndex;
                    scanf("%d", &delIndex);
                    if (delIndex == pCounter + 1)
                        break;
                    deletePlaylist(playlists, delIndex - 1, pCounter);
                    pCounter--;
                    if (pCounter > 0) {
                        playlists = realloc(playlists, sizeof(Playlist) * pCounter);
                    }
                    break;
                default:
                    printf("Invalid input\n");
                break;
            }
        } else {
            printf("Invalid input\n");
        }
    } while (task != 4);
    return 0;
}

void sortYear(Song** songs, int sNum) {
    for (int i = 0; i < sNum - 1; i++) {
        for (int j = 0; j < sNum - i - 1; j++) {
            if (songs[j]->year > songs[j + 1]->year) {
                Song* temp = songs[j];
                songs[j] = songs[j + 1];
                songs[j + 1] = temp;
            }
        }
    }
    printf("sorted\n");
}

void sortStreamsAscend(Song** songs, int sNum) {
    for (int i = 0; i < sNum - 1; i++) {
        for (int j = 0; j < sNum - i - 1; j++) {
            if (songs[j]->streams > songs[j + 1]->streams) {
                Song* temp = songs[j];
                songs[j] = songs[j + 1];
                songs[j + 1] = temp;
            }
        }
    }
    printf("sorted\n");
}

void sortStreamsDescend(Song** songs, int sNum) {
    for (int i = 0; i < sNum - 1; i++) {
        for (int j = 0; j < sNum - i - 1; j++) {
            if (songs[j]->streams < songs[j + 1]->streams) {
                Song* temp = songs[j];
                songs[j] = songs[j + 1];
                songs[j + 1] = temp;
            }
        }
    }
    printf("sorted\n");
}

void sortAlpha(Song** songs, int sNum) {
    for (int i = 0; i < sNum - 1; i++) {
        for (int j = 0; j < sNum - i - 1; j++) {
            if (strcmp(songs[j + 1]->title, songs[j]->title) < 0) {
                Song* temp = songs[j];
                songs[j] = songs[j + 1];
                songs[j + 1] = temp;
            }
        }
    }
    printf("sorted\n");
}

void deletePlaylist(Playlist* playlist, int dIndex, int pNum) {
    for (int i = 0; i < playlist[dIndex].songsNum; i++) {
        freeSong(playlist[dIndex].songs[i]);
    }
    free(playlist[dIndex].name);
    free(playlist[dIndex].songs);
    for (int i = dIndex; i < pNum - 1; i++) {
        playlist[i] = playlist[i + 1];
    }
    printf("Playlist deleted.\n");
}

void deleteSong(Playlist* playlist, Song** songs, int dIndex) {
    freeSong(songs[dIndex]);
    for (int i = dIndex; i < playlist->songsNum - 1; i++) {
        songs[i] = songs[i + 1];
    }
    playlist->songsNum--;
    songs[dIndex] = NULL;
    printf("Song deleted successfully.\n");
}

void freeSong(Song* song) {
    free(song->artist);
    free(song->title);
    free(song->lyrics);
    free(song);
}

void freePlaylists(Playlist* playlist, int* pNum) {
    for (int i = 0; i < *pNum; i++) {
        for (int j = 0; j < playlist[i].songsNum; j++) {
            freeSong(playlist[i].songs[j]);
        }
        free(playlist[i].songs);
        free(playlist[i].name);
    }
}

void printMainMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

void printPlaylistsMenu(Playlist *playlists, int counter) {
    printf("Choose a playlist:\n");
    for (int i = 0; i < counter; i++) {
        printf("%d. %s\n", i + 1, playlists[i].name);
    }
    printf("%d. Back to main menu\n", ++counter);
}

void addPlaylist(Playlist **playlists, int index) {
    printf("Enter playlist's name:\n");
    int stringSize = STRING_SIZE, i = 0;
    char *playlistName = (char *)malloc(stringSize * sizeof(char));
    if (playlistName == NULL)
        exit(1);
    char temp;
    char discard;
    // Read the newline from previous input
    scanf("%c", &discard);
    while (scanf("%c", &temp) == 1 && temp != '\n') {
        if (temp == '\r')
            continue;
        // Expand memory if necessary
        if (i >= stringSize - 2) {
            stringSize += stringSize;
            playlistName = (char*)realloc(playlistName, stringSize * sizeof(char));
            if (playlistName == NULL)
                exit(1);
        }
        playlistName[i++] = temp;
    }
    playlistName[i] = '\0';
    // Set playlists[index].name to the string length
    (*playlists)[index].name = malloc((strlen(playlistName) + 1) * sizeof(char));
    if ((*playlists)[index].name == NULL)
        exit(1);
    // Copy the string to the playlists array
    strcpy((*playlists)[index].name, playlistName);
    free(playlistName);
    (*playlists)[index].songsNum = 0;
    (*playlists)[index].songs = NULL;
}

void printSinglePlaylistMenu() {
    printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
}
// Prints the songs of the playlist the user chose
void showPlaylist(Playlist *playlists, int index) {
    for (int i = 0; i < playlists[index].songsNum; i++) {
        printf("%d. Title: %s\nArtist: %s\nReleased: %d\nStreams: %d\n\n"
            , i + 1, playlists[index].songs[i]->title, playlists[index].songs[i]->artist,
            playlists[index].songs[i]->year, playlists[index].songs[i]->streams);
    }
}
// Plays all the songs of a playlist
void playAllSongs(Playlist *playlist, int pIndex) {
    for (int i = 0; i < playlist[pIndex].songsNum; i++) {
        printf("Now playing %s:\n", playlist[pIndex].songs[i]->title);
        printf("$ %s $\n", playlist[pIndex].songs[i]->lyrics);
        playlist[pIndex].songs[i]->streams++;
        printf("\n");
    }
}
// Plays the song that the user chose
void playSong(Playlist *playlist, int pIndex, int sIndex) {
    printf("Now playing %s:\n", playlist[pIndex].songs[sIndex]->title);
    printf("$ %s $\n", playlist[pIndex].songs[sIndex]->lyrics);
    playlist[pIndex].songs[sIndex]->streams++;
}
void addSong(Playlist **playlist, int pIndex, int sIndex) {
    printf("Enter song's details\nTitle:\n");
    int stringSize = STRING_SIZE, i = 0;
    char *title = (char *)malloc(stringSize * sizeof(char));
    if (title == NULL)
        exit(1);
    char temp;
    scanf("%*c");
    while (scanf("%c", &temp) == 1 && temp != '\n') {
        if (temp == '\r')
            continue;
        // Expand memory if necessary
        if (i >= stringSize - 2) {
            stringSize += stringSize;
            title = (char*)realloc(title, stringSize * sizeof(char));
            if (title == NULL)
                exit(1);
        }
        title[i++] = temp;
    }
    title[i] = '\0';
    // Set song title to the string length
    (*playlist)[pIndex].songs[sIndex]->title = malloc((strlen(title) + 1) * sizeof(char));
    if ((*playlist)[pIndex].songs[sIndex]->title == NULL)
        exit(1);
    // Copy the string to song's title
    strcpy((*playlist)[pIndex].songs[sIndex]->title, title);
    free(title);
    printf("Artist:\n");
    stringSize = STRING_SIZE;
    char *artist = (char *)malloc(stringSize * sizeof(char));
    i = 0;
    if (artist == NULL)
        exit(1);
    while (scanf("%c", &temp) == 1 && temp != '\n') {
        if (temp == '\r')
            continue;
        // Expand memory if necessary
        if (i >= stringSize - 2) {
            stringSize += stringSize;
            artist = (char*)realloc(artist, stringSize * sizeof(char));
            if (artist == NULL)
                exit(1);
        }
        artist[i++] = temp;
    }
    artist[i] = '\0';
    // Set song artist to the string length
    (*playlist)[pIndex].songs[sIndex]->artist = malloc((strlen(artist) + 1) * sizeof(char));
    if ((*playlist)[pIndex].songs[sIndex]->artist == NULL)
        exit(1);
    // Copy the string to song's artist
    strcpy((*playlist)[pIndex].songs[sIndex]->artist, artist);
    free(artist);

    printf("Year of release:\n");
    int year;
    scanf("%d", &year);
    (*playlist)[pIndex].songs[sIndex]->year = year;

    printf("Lyrics:\n");
    stringSize = STRING_SIZE;
    i = 0;
    char *lyrics = (char *)malloc(stringSize * sizeof(char));
    if (lyrics == NULL)
        exit(1);
    scanf("%*c");
    while (scanf("%c", &temp) == 1 && temp != '\n') {
        if (temp == '\r')
            continue;
        // Expand memory if necessary
        if (i >= stringSize - 2) {
            stringSize += stringSize;
            lyrics = (char*)realloc(lyrics, stringSize * sizeof(char));
            if (lyrics == NULL)
                exit(1);
        }
        lyrics[i++] = temp;
    }
    lyrics[i] = '\0';
    // Set song lyrics to the string length
    (*playlist)[pIndex].songs[sIndex]->lyrics = malloc((strlen(lyrics) + 1) * sizeof(char));
    if ((*playlist)[pIndex].songs[sIndex]->lyrics == NULL)
        exit(1);
    // Copy the string to song's lyrics
    strcpy((*playlist)[pIndex].songs[sIndex]->lyrics, lyrics);
    free(lyrics);
}