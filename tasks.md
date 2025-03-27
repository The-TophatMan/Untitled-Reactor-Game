# Gauges (0%)
- [ ] Minimum Value
- [ ] Maximum Value
- [ ] Labels
- [ ] Units
- [ ] Pointer
- [ ] Minor Ticks
- [ ] Major Ticks
## Shapes (0%)
- [ ] Circular
- [ ] Horizontal Bar
- [ ] Vertical Bar
## Coloring (0%)
- [ ] Low Caution
- [ ] Low Warning
- [ ] High Caution
- [ ] High Warning
- [ ] None


# Switches (0%)
- [ ] User Defined Options
- [ ] Label
- [ ] Units
- [ ] Pointer
- [ ] Enabled Light?
- [ ] Breaker Mode

# Annunciators (0%)
- [ ] Caution Text Color
- [ ] Warning Text Color
- [ ] Info Text Color
- [ ] Lit Caution Highlight
- [ ] Lit Warning Highlight
- [ ] Lit Info Highlight
- [ ] Acknowledge Button
- [ ] Silence Button?

# Separation (0%)
- [ ] Panel Label
- [ ] Separation Lines

# Game (0%)
- [ ] Escape Key
- [ ] Error Handling
- [ ] Draw to Texture (Ignore pointers)
- [ ] Audio
## Loading Screen (0%)
- [ ] Progress (i.e. 12/17)
- [ ] Current Major Task (i.e. Loading Assets)
- [ ] Current Minor Task (i.e. Drawing Gauge 1)
## Menu (0%)
- [ ] Tutorial
- [ ] Layout based on screen size
- [ ] Screen Modes
- [ ] Audio Settings

# Else (29%)
- [ ] Everything except pointers can be saved as textures
- [ ] Save everything as its individual texture and add them to a rendering vector
- [ ] Switches need interaction hierarchy
- [ ] Scene needs to be redrawn on screen size change
- [ ] Everything needs to be positioned and scaled relative to window size
- [x] Error handling needs to be able to return error codes and path structures
- [x] Executable should allow for different logging levels

# Error Handling (100%)
- [x] Message (i.e. Invalid Pointer)
- [x] Code (i.e. 7)
- [x] Path Structure (i.e. THIS::THAT::FUNCTION)
- [x] Severity (i.e. Fatal, Warning, Caution, Info)