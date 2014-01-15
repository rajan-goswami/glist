
**glist**
-----


----------


This is generic list library written in C. It is not generic in strict sense and can never be so in C. However it is a data structure carrying client provided type information of data. In that way, client is in charge of inserting [Data + Type] node into list. All list functions insert, delete, edit, clear etc. functions are supported through a List Service.

Client can store hybrid data inside list, but they should be aware of data type of data they are dealing with. It hides the complexity of list implementation and gives client a token to work with the list. To operate on the list, client has to use given token (an integer). Client must free the resources after using the list.


----------


**Usage**
---------

 - Get List service handle

>  `struct generic_list_service *list_service =
> glist_create_list_service();`

 - Creating a New List

> `unsigned int list_id = list_service->create_empty_list();`

 - Creation of Data
 
> `data_t data;`
> 
>  `data.type_id = INT; // INT is type-id of int`
> 
>  `int *ii = (int *) calloc((size_t) 1, sizeof (int));`
> 
>  `*ii = 10;`
> 
>  `data.p = ii;`

 - Inserting data

> `int i = list_service->insert(list_id, data)`

 - Deleting data

> `data_t data;`

> `list_service->delete_node(list_id, 0, &data)`

> `free(data.p);`

 - Retrieve data
 
> `data_t data;`

> `list_service->get_data(list_id, index, &data)`

> `printf("\tData at %d index : %d \n\n", index, *((int *) data.p));`