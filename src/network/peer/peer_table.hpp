#pragma once

#include <tabulate/table.hpp>
#include <map>
#include <string>
#include <vector>

#include "peer.hpp"

using namespace std;

class PeerTable {
   map<int, Peer> table;

public: 
   void add_peer(int id, string ip) {
      table.insert(pair(id, Peer(id, ip)));
   }

   Peer get_peer(int id) {
      return table.at(id);
   }

   void remove_peer(int id) {
      table.erase(id);
   }

   map<int, Peer> get_private_table() {
      return table;
   }

   vector<int> get_public_table() {
      vector<int> keys = vector<int>();
      for (auto peer : table) {
         keys.push_back(peer.first);
      }

      return keys;
   }

   int get_size() {
      return table.size();
   }

   friend ostream& operator<< (ostream& os, const PeerTable& peerTable) {
      tabulate::Table t;

      t.add_row({"ID", "Adresse IP"});
      
      for (const auto& [id, peer] : peerTable.table) {
         t.add_row({
               to_string(id), 
               peer.ip
         });
      }

      t[0].format()
         .font_color(tabulate::Color::yellow)
         .font_align(tabulate::FontAlign::center)
         .font_style({tabulate::FontStyle::bold});

      t.column(0).format().font_align(tabulate::FontAlign::right);
      t.column(1).format().font_align(tabulate::FontAlign::left);

      return os << t;
   };
};