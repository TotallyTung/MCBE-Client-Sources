
enum TextPacketType : unsigned char {
  Raw = 0x0000,
  Chat = 0x0001,
  Translate = 0x0002,
  Popup = 0x0003,
  JukeboxPopup = 0x0004,
  Tip = 0x0005,
  SystemMessage = 0x0006,
  Whisper = 0x0007,
  Announcement = 0x0008,
  TextObjectWhisper = 0x0009,
  TextObject = 0x000a,
};
