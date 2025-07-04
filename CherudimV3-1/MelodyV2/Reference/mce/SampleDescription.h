
class mce::SampleDescription { /* Size=0x8 */
  /* 0x0000 */ int32_t count;
  /* 0x0004 */ int32_t quality;
  
  bool operator==(const mce::SampleDescription&) const;
  SampleDescription();
};
