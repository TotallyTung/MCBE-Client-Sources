
enum GuiMessage::MessageType : int32_t {
  None = 0x0000,
  ChatMessage = 0x0001,
  ClientMessage = 0x0002,
  LocalizedMessage = 0x0003,
  SystemMessage = 0x0004,
  WhisperMessage = 0x0005,
  TextObjectMessage = 0x0006,
  TextObjectWhisperMessage = 0x0007,
  AnnouncementMessage = 0x0008,
};
