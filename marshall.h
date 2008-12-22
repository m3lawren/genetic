#ifndef _MARSHALL_H_
#define _MARSHALL_H_

void saveState(const char*, const History&, const Config&);
void loadState(const char*, History&, Config&);

#endif
