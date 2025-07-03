struct OffsetsComponent {
    float mHeightOffset;
    float mExplosionOffset;
    Vec3 mHeadOffset;
    Vec3 mDropOffset;
    Vec3 mEyeOffset;
    Vec3 mMouthOffset;
    Vec3 mBreathingOffset;
  public:
    bool operator==(const OffsetsComponent &) const;
}
