This is a Generic List implementation in C.

Client can store hybrid data inside list, but they should 
be aware of data type of data they are dealing with.
It hides the complexity of list implementation and gives 
client a token to work with the list. To operate on the list, 
client has to use given token (an integer).

Client can access all standard List functions. Client must free
the resources after using the list.
