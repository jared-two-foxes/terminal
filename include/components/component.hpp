#ifndef FRAMEWORK_COMPONENT_HPP__
#define FRAMEWORK_COMPONENT_HPP__

#include <framework/terminal/image.hpp>
#include <framework/terminal/components/text.hpp>

#include <string>
#include <memory>

namespace framework {

struct Renderable {
  virtual Image render(unsigned const w) const = 0;
};

inline auto isImage(Image const& x) { return x; }

template <class R>
auto getImage(unsigned const w, R const& r) -> decltype(isImage(r.render(w))) {
  return r.render(w);
}

template <class R>
auto getImage(unsigned const w, R const& r) -> decltype(isImage(r(w))) {
  return r(w);
}

template <class R>
auto getImage(unsigned const w, R const& r) -> decltype(isImage(r)) {
  return r(w);
}

template <class R>
auto getImage(unsigned const w, R const& r) -> decltype(Text{{},toString(r)}.render(w)) {
  return Text{{}, toString(r)}.render(w);
}

struct Component : Renderable {

  template <class T>
  Component(T const& model) :
    ptr{std::make_shared<Model<T>>(model)}
  {}

  virtual Image render(unsigned const w) const override {
    return ptr->render(w);
  }

  template <class T>
  struct Model : Renderable {
  	T const model;

  	Model(T const& data) : model{data} {}

  	virtual Image render(unsigned const w) const override {
  		return getImage(w, model);
  	}
  };

  std::shared_ptr<Renderable > ptr;

};

}

#endif // FRAMEWORK_COMPONENT_HPP__
