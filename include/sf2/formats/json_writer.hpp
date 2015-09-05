/***********************************************************\
 * JSON reader and writer                                  *
 *     ___________ _____                                   *
 *    /  ___|  ___/ __  \                                  *
 *    \ `--.| |_  `' / /'                                  *
 *     `--. \  _|   / /                                    *
 *    /\__/ / |   ./ /___                                  *
 *    \____/\_|   \_____/                                  *
 *                                                         *
 *                                                         *
 *  Copyright (c) 2014 Florian Oetke                       *
 *                                                         *
 *  This file is part of SF2 and distributed under         *
 *  the MIT License. See LICENSE file for details.         *
\***********************************************************/

#pragma once

#include <string>
#include <ostream>
#include <vector>
#include <cassert>

namespace sf2 {
namespace format {

	class Json_writer {
		public:
			Json_writer(std::ostream& stream);

			void begin_document();
			void begin_obj();
			void begin_array();
			void end_current();

			void write_nullptr();

			void write(const char*);
			void write(const std::string&);
			void write(bool);
			void write(float);
			void write(double);
			void write(uint8_t);
			void write(int8_t);
			void write(uint16_t);
			void write(int16_t);
			void write(uint32_t);
			void write(int32_t);
			void write(uint64_t);
			void write(int64_t);

		private:
			void newline() {
				_stream<<std::endl;
				for(std::size_t i=0; i<_state.size(); ++i)
					_stream<<"    ";
			}

			void _pre_write() {
				if(_state.empty())
					return;

				switch(_state.back()) {
					case State::first_obj_key:
						_state.back() = State::obj_key;
						break;
					case State::first_array:
						_state.back() = State::array;
						break;

					case State::obj_value:
						break;

					case State::obj_key:
					case State::array:
						_stream<<",";
						newline();
						break;
				}
			}
			void _post_write() {
				if(_state.empty())
					return;

				if(_state.back()==State::obj_value) {
					_state.back()=State::obj_key;

				} else if(_state.back()==State::obj_key) {
					_stream<<": ";
					_state.back()=State::obj_value;
				}
			}

			template<class T>
			void _write(const T& v) {
				_pre_write();

				_stream<<v;

				_post_write();
			}

			enum class State {
				first_obj_key, obj_key, obj_value, first_array, array
			};

			std::ostream& _stream;
			std::vector<State> _state;
	};

	Json_writer::Json_writer(std::ostream& stream) : _stream(stream) {
	}

	void Json_writer::begin_document() {
		_stream<<"{";
		_state.push_back(State::first_obj_key);
		newline();
	}

	void Json_writer::end_current() {
		auto closed = _state.back();
		_state.pop_back();
		newline();

		switch(closed) {
			case State::obj_value:
				assert(!"end_current afer object key: missing value");
				break;

			case State::first_obj_key:
			case State::obj_key:
				_write("}");
				break;

			case State::first_array:
			case State::array:
				_write("]");
				break;
		}

		if(_state.empty())
			_stream<<std::endl;
	}

	void Json_writer::begin_obj() {
		_stream<<"{";
		_state.push_back(State::first_obj_key);
		newline();
	}

	void Json_writer::begin_array() {
		_stream<<"[";
		_state.push_back(State::first_array);
		newline();
	}

	void Json_writer::write_nullptr() {
		_write("null");
	}

	void Json_writer::write(const char* v) {
		_pre_write();

		_stream.put('"');

		std::size_t i=0;
		char c = v[i];
		while(c!='\0') {
			if(c=='"')
				_stream.put('\\');

			_stream.put(c);
			c = v[++i];
		}

		_stream.put('"');

		_post_write();
	}

	void Json_writer::write(const std::string& v) {
		write(v.c_str());
	}

	void Json_writer::write(bool v) {
		_write(v ? "true" : "false");
	}

	void Json_writer::write(float v) {
		_write(v);
	}

	void Json_writer::write(double v) {
		_write(v);
	}

	void Json_writer::write(uint8_t v) {
		_write(v);
	}

	void Json_writer::write(int8_t v) {
		_write(v);
	}

	void Json_writer::write(uint16_t v) {
		_write(v);
	}

	void Json_writer::write(int16_t v) {
		_write(v);
	}

	void Json_writer::write(uint32_t v) {
		_write(v);
	}
	void Json_writer::write(int32_t v) {
		_write(v);
	}

	void Json_writer::write(uint64_t v) {
		_write(v);
	}

	void Json_writer::write(int64_t v) {
		_write(v);
	}

}
}