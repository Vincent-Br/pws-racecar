package main

import (
	"fmt"
	"net/http"

	"github.com/gorilla/websocket"
)

type Message struct {
	Type      int   `json:"type"`
	Timestamp int64 `json:"timestamp"`
	Data      Data  `json:"data"`
}

type Data struct {
}

type Conn struct {
	ws *websocket.Conn
	send chan []byte
	h *Hub
}

type Hub struct {
	conns map[*Conn]bool
}

func main() {
	wsUpgrader := websocket.Upgrader{
		ReadBufferSize:  1024,
		WriteBufferSize: 1024,
	}

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		conn, err := wsUpgrader.Upgrade(w, r, nil)

		if err != nil {
			fmt.Println(err)
			return
		}

		for {
			var message Message

			err = conn.ReadJSON(&message)

			if err != nil {
				fmt.Println(err)
				return
			}

			fmt.Println("Received:", message)

			err = conn.WriteJSON(message)

			if err != nil {
				fmt.Println(err)
				return
			}

		}
	})

	http.ListenAndServe(":4000", nil)
}
