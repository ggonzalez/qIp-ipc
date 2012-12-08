Quick, sImple but Powerfull IPC Mechanism
=========================================

Version 1.0:

Goals:

- Fast message passing mechanism (Embedded Systems in Mind)

- Provide a simple to use but still powerful IPC

- Hot-Plug, restarting a server shouldn't tear the whole network down

- MultiNode, allow entities to be anywhere in a network

Features:

- The whole library relays on UDP which allows an easy hot-plugging mechanism

- This version doesn't support auto-retrasmission but it is simple to be
implemented by the client using the provided API

- Clients can ask NodeManagers for another peer's address

- Programmers can go straight an used hardcoded network/ports address if required
