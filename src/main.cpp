/**
 * Copyright 2020 Blanovschi Dinu

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
#include <SFML/Graphics.hpp>
#include <Logger.hpp>
#include <iostream>

struct ColorElem {
  int v;
  std::string country_name;
  std::string link_name;
  sf::RenderTexture tex;
  sf::Sprite sprite;
  std::wstring country_desc;
} PElem[100];

constexpr size_t WinW = 1300, WinH = 1000;
constexpr size_t DetailTexW = 550, DetailTexH = 500;

void ReadDataMap(std::wifstream &dmap, std::wstring &str);
int main() {
  sf::Texture map_texture, color_map_texture;
  DEU::Logger logger;
  if (!map_texture.loadFromFile("res/Eu_map.png") || !color_map_texture.loadFromFile("res/Eu_map_colored.png")) {
	DEU::Logger::Log(DEU::FATAL, "Cannot load eu maps");
	return 1;
  }
  map_texture.setSmooth(true);
  sf::Sprite map_sprite, colored_map_sprite;
  map_sprite.setTexture(map_texture);
  colored_map_sprite.setTexture(color_map_texture);
  sf::Vector2f scale_vec =
	  {static_cast<float>(WinW) / map_texture.getSize().x, static_cast<float>(WinH) / map_texture.getSize().y};
  map_sprite.setScale(scale_vec);
  colored_map_sprite.setScale(scale_vec);
  sf::Image im = colored_map_sprite.getTexture()->copyToImage();
  sf::Font font, titleFont;
  if (!titleFont.loadFromFile("res/arial.ttf") || !font.loadFromFile("res/FreeSerif.ttf")) {
	DEU::Logger::Log(DEU::FATAL, "cannot load font");
	return 1;
  }

  {
	int i = 0;
	std::ifstream fin("res/colormap");
	std::wifstream data_map("res/datamap");
	data_map.imbue(std::locale("en_US.UTF-8"));
	while (fin >> PElem[i].v) {
	  fin.ignore();
	  std::getline(fin, PElem[i].country_name);
	  ReadDataMap(data_map, PElem[i].country_desc);
	  PElem[i].tex.create(DetailTexW, DetailTexH);
	  PElem[i].tex.clear(sf::Color::Black);
	  {
		sf::Text text;
		text.setFont(titleFont);
		text.setString(PElem[i].country_name);
		text.setPosition(10, 10);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);
		text.setStyle(sf::Text::Bold);
		PElem[i].tex.draw(text);
	  }
	  {
		sf::Text text;
		text.setFont(font);
		text.setString(PElem[i].country_desc);
		text.setPosition(10, 38);
		text.setCharacterSize(15);
		text.setFillColor(sf::Color::White);
		//		text.setStyle(sf::Text::Bold);
		PElem[i].tex.draw(text);
	  }
	  PElem[i].tex.display();
	  PElem[i].sprite.setTexture(PElem[i].tex.getTexture());
	  i++;
	}
	fin.close();
	data_map.close();
  }

  sf::RenderWindow window(sf::VideoMode(WinW, WinH), "Discover EU", sf::Style::Titlebar | sf::Style::Close);
  int selected = -1;
  int mouseX = 0, mouseY = 0;
  while (window.isOpen()) {
	sf::Event event;
	while (window.pollEvent(event)) {
	  if (event.type == sf::Event::Closed) {
		window.close();
	  } else if (event.type == sf::Event::MouseButtonPressed) {
		sf::Vector2i pos{event.mouseButton.x, event.mouseButton.y};
		sf::Color c = im.getPixel(pos.x / scale_vec.x, pos.y / scale_vec.y);
		if (c.a <= 10) {
		  selected = -1;
		  continue;
		}
		selected = (255 - (int)c.r) / 4;
		if (selected > 30) {
		  selected = -1;
		  continue;
		}
		std::cout << "Clicked on \"" << PElem[selected].country_name << "\"" << std::endl;
	  } else if (event.type == sf::Event::MouseMoved) {
		mouseX = event.mouseMove.x, mouseY = event.mouseMove.y;
		sf::Color c = im.getPixel(mouseX / scale_vec.x, mouseY / scale_vec.y);
		if (c.a <= 10) {
		  selected = -1;
		  continue;
		}
		selected = (255 - (int)c.r) / 4;
		if (selected > 30) {
		  selected = -1;
		  continue;
		}
	  }
	}
	window.clear(sf::Color{0, 0x69, 0x94});
	window.draw(map_sprite);
	if (selected != -1) {
	  int px = mouseX - DetailTexW, py = mouseY - DetailTexH;
	  if (px < 0)
		px = mouseX;
	  if (py < 0)
		py = 0;
	  PElem[selected].sprite.setPosition(px, py);
	  window.draw(PElem[selected].sprite);
	}
	window.display();
  }
  return 0;
}

void ReadDataMap(std::wifstream &dmap, std::wstring &str) {
  str = L"";
  std::wstring x = L"";
  while (x != L"{") {
	std::getline(dmap, x);
  }
  std::getline(dmap, x);
  while (x != L"}") {
	str += x + L"\n";
	std::getline(dmap, x);
  }
}

