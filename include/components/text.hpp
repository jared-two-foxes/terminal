#ifndef FRAMEWORK_TEXT_HPP__
#define FRAMEWORK_TEXT_HPP__

#include <terminal/image.hpp>
#include <terminal/util.hpp>
#include <terminal/reflow.hpp>
#include <terminal/style.hpp>

#include <algorithm>
#include <string>

namespace framework {

struct Text {
  Style const style;
  std::string const content;

  template <class T>
  Text(Style const& style, T const& content) :
  	style{style},
  	content{foundation::toString(content)}
  {}

  template <class T>
  Text(T const& content) :
    style{Style{}},
    content{foundation::toString(content)}
  {}

  Image render(unsigned const maxWidth) const {
  	auto const lines = foundation::split(reflow(maxWidth, content), "\n");
  	auto const height = lines.size();
    auto const widest_element = std::max_element( lines.begin(), lines.end(),
  	  [](auto const& a, auto const& b) { return a.size() < b.size(); });
    auto const width = widest_element != lines.end() ? widest_element->size() : 0;

  	auto image = Image::create(width, height, Pixel{'\0', style});
  	unsigned y = 0;
  	for (auto const& line : lines) {
  	  for (int x = 0; x < line.size(); ++x) {
  	  	image(x,y).c = line[x];
  	  }
  	  ++y;
  	}

  	return image;
  }
};

}

#endif // FRAMEWORK_TEXT_HPP__
