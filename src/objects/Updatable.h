#ifndef UPDATABLE_H
#define UPDATABLE_H

class Updatable {
public:
  virtual void Update(float elapsedTimeMs) = 0;
};

#endif // UPDATABLE_H