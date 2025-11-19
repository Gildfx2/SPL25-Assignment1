#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}

/**
 * Destructor
 */
DJLibraryService::~DJLibraryService(){
    for(AudioTrack* track : library){
        delete track;
    }
    library.clear();
    // playlist is allocated on the heap and therefore will be deleted using his destructor automatically
}

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    AudioTrack* track;
    for(int i = 0 ; i < library_tracks.size() ; i++){
        if(library_tracks[i].type == "MP3"){
            AudioTrack* track = new MP3Track(library_tracks[i].title, library_tracks[i].artists, library_tracks[i].duration_seconds, library_tracks[i].bpm, 
                library_tracks[i].extra_param1, library_tracks[i].extra_param2);
            std::cout << "MP3Track created: " << library_tracks[i].extra_param1 << " kbps" << std::endl;
            library.push_back(track);
        }
        else if(library_tracks[i].type == "WAV"){
            AudioTrack* track = new WAVTrack(library_tracks[i].title, library_tracks[i].artists, library_tracks[i].duration_seconds, library_tracks[i].bpm, 
                library_tracks[i].extra_param1, library_tracks[i].extra_param2);
            std::cout << "WAVTrack created: " << library_tracks[i].extra_param1 << "Hz/" << library_tracks[i].extra_param2 << "bit" << std::endl;
            library.push_back(track);
        }
    }
    std::cout << "[INFO] Track library built: " << library_tracks.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    AudioTrack* track = playlist.find_track(track_title);
    return track;
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << " [INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    int counter = 0;
    for(int i = 0 ; i < track_indices.size() ; i++){
        if(track_indices[i] > 0 && track_indices[i] <= library.size()){
            AudioTrack* tmp_track = library[track_indices[i]-1];
            AudioTrack* cloned_track = tmp_track->clone().release();
            if(cloned_track == nullptr){
                std::cout << "[ERROR] Track is null" << std::endl;
            }
            else{
                cloned_track->load();
                cloned_track->analyze_beatgrid();
                playlist.add_track(cloned_track);
                //std::cout << "Added ’" << cloned_track->get_title() << "’ to playlist ’" << playlist_name << "’" << std::endl;
                counter++;
            }
        }
        else{
            std::cout << "[WARNING] Invalid track index: " << track_indices[i] << std::endl;
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << "(" << counter << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> track_titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for(AudioTrack* track : tracks){
        track_titles.push_back(track->get_title());
    }
    return track_titles;
}
