#ifndef PEER_MANAGER_H
#define PEER_MANAGER_H

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "torrent_runtime.h"

uint16_t DEBUG_CURRENTLY_DOWNLOADING;

struct Peer{
    int socket;
    uint16_t port; //Also in big endian format
    uint8_t address[4]; //The corresponding 64bits version is in big endian format. 
    uint8_t am_choking, am_interested, peer_choking, peer_interested;
    uint8_t *bitfield; //a dynamically allocated array, 
    int bitfield_length; //how many bytes the bitfield uses
    int handshaked; //whether this peer has been handshaked
    struct timeval last_received_message_time;
    struct timeval last_sent_message_time;
    struct Peer *next;
    
    // Upload & Download Fields
    struct timeval download_req_sent_time;
    uint64_t download_rate; 
    uint8_t curr_dl; // 1=downloading, 0=not downloading
    uint8_t curr_dl_begin; // did the download begin
    uint32_t curr_dl_next_subpiece; // 0-based index of last subpiece downloaded
    uint64_t curr_dl_piece_idx;
    uint8_t curr_up; // 1=uploading, 0=not uploading
    uint64_t curr_up_piece_idx;

    uint8_t connect_to_use; // 1 for peer connecting with me, 0 for me connecting with them
};

//returns the root peer of the peer linked list
struct Peer *peer_manager_get_root_peer();

//number of bytes in each piece (integer)
uint8_t get_piece_length();

//get the list of four peers which we unchoked, used for uploading & downloading (choking algorithm)
struct Peer *get_am_unchoked();

//update the download rate for the peer, the peer manager doesnt call it
void update_download_rate(struct Peer *peer, uint64_t download_rate);

//returns the peer from the given socket
struct Peer *get_peer_from_socket(int socket);

//starts the peer manager
int start_peer_manager(Torrent *torrent);

//Returns 0 if theres no such peer; 1 otherwise
int peer_manager_inform_disconnect(struct Peer *peer);

/*
    Called by the Piece Manager. This function will communicate with a peer
    and request to download a piece by sending request message

    Returns 1 if socket error / peer is choking us
    Returns 0 if success
*/
int peer_manager_begin_download(struct Peer* peer, int pieceIndex);

/*
    Params:
    ----------------------------------------------------------------
    is_upload   - 1 if this was an upload, 0 if it was a download
    peer        - A pointer to the peer
    pieceIndex  - The index of the piece we uploaded/downloaded successfully

    Called by the Piece Manager when a piece has been downloaded from
    a peer and we are no longer downloading from this peer. Updates the
    Peer.curr_dl / Peer.curr_up / Peer.curr_dl_piece / Peer.curr_up_piece
    fields and any choking algorithm.
*/
void peer_manager_upload_download_complete(uint8_t is_upload, struct Peer* peer, int piece_index);

//disconnects from all peers
void peer_manager_complete();

//update the download rate for the peer, the peer manager doesnt call it
int peer_manager_update_download_rate(struct Peer *peer, uint64_t download_rate);


#endif
