#ifndef MESSAGE_HUB_DECLARATIONS_H
#define MESSAGE_HUB_DECLARATIONS_H

#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Common/Pool/ManagedChunk.h"
#include "Vajra/Common/Pool/Pool.h"

#define MessageChunk ManagedChunk<Message>
// TODO [Implement] Overload the "->" operator instead, maybe
#define MessageChunk_Message(message_chunk) message_chunk.GetChunk()->meat

#endif // MESSAGE_HUB_DECLARATIONS_H

