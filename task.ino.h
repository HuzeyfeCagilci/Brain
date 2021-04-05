
#include "task.h"

void SSil(Service_node**head, byte id)
{
	Node_return ret = Service_node_delete(head, id);
	switch(ret)
	{
		case Success:
		Serial.print("Succesfully deleted node whose id is ");
		Serial.println(id);
		break;
		case Cannot_find:
		Serial.println("There isn't node whose id is ");
		Serial.println(id);
		break;
		default:break;
	}
}