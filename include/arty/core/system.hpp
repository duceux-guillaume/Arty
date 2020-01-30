#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <arty/core/input.hpp>
#include <arty/core/memory.hpp>
#include <arty/core/result.hpp>

namespace arty {

class System {
 public:
  virtual ~System() {}

  virtual Result process(Ptr<Memory> const& board,
                         Ptr<InputManager> const& inputs);
  virtual Result process(Ptr<Memory> const& board);
  virtual Result init(Ptr<Memory> const& board,
                      Ptr<InputManager> const& inputs);
  virtual Result init(Ptr<Memory> const& board);
  virtual void release();

 protected:
};

class EventSystem : public System {
 public:
  using job_func = std::function<Result(Ptr<Memory> const& board)>;

  EventSystem(Input const& input, Event const& event, job_func job)
      : _input(input), _event(event), _job(job){};

  Result process(Ptr<Memory> const& board,
                 Ptr<InputManager> const& inputs) override;
  Result init(Ptr<Memory> const& board,
              Ptr<InputManager> const& inputs) override;

 private:
  Input _input;
  Event _event;
  job_func _job;
};

class SimpleSystem : public System {
 public:
  using job_func =
      std::function<Result(Ptr<Memory> const&, Ptr<InputManager> const&)>;

  SimpleSystem(job_func job) : _job(job) {}
  Result process(Ptr<Memory> const& mem,
                 Ptr<InputManager> const& inputs) override {
    return _job(mem, inputs);
  }

 private:
  job_func _job;
};

};  // namespace arty

#endif  // SYSTEM_HPP
