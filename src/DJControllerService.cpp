#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
    bool exist_flag = cache.contains(track.get_title());
    if(exist_flag){
        cache.get(track.get_title());
        return 1;
    }
    else{
        PointerWrapper<AudioTrack> ptr_track = track.clone();
        if(!ptr_track) {
            std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
            return 2; 
        }
        ptr_track->load();
        ptr_track->analyze_beatgrid();
        bool insert_flag = cache.put(std::move(ptr_track));
        if(insert_flag) return -1;
        else return 0;
    }
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    return cache.get(track_title); // Placeholder
}
