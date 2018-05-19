#include "rlp.h"

#include <cstring>
#include <cstdint>
#include<stdexcept>

using namespace std;


RLP::RLP(const vector<uint8_t> & contents)
	: RLP{contents, 0, contents.size()}
{}

RLP::RLP(const vector<uint8_t> & contents, size_t offset, size_t maxLength)
	: _contents{contents}, _prefixOff{offset}
{
	if (maxLength == 0)
		throw BadRLPFormat();

	uint8_t prefix = _contents[_prefixOff];
	if (prefix < 128) {
		_dataLen = 1;
		_totalLen = 1;
		_dataOff = _prefixOff;

	} else if (prefix <= 183) {
		_dataLen = prefix - 128;
		_totalLen = 1 + _dataLen;
		_dataOff = _prefixOff + 1;

	} else if (prefix < 192) {
		size_t dataLengthSize = prefix - 183;
		if (maxLength < 1 + dataLengthSize)
			throw BadRLPFormat();

		parseDataLength(dataLengthSize);
		_totalLen = 1 + dataLengthSize + _dataLen;
		_dataOff = _prefixOff + 1 + dataLengthSize;

	} else if (prefix <= 247) {
		_dataLen = prefix - 192;
		_totalLen = 1 + _dataLen;
		_dataOff = _prefixOff + 1;

	} else {
		size_t dataLengthSize = prefix - 247;
		if (maxLength < 1 + dataLengthSize)
			throw BadRLPFormat();

		parseDataLength(dataLengthSize);
		_totalLen = 1 + dataLengthSize + _dataLen;
		_dataOff = _prefixOff + 1 + dataLengthSize;
	}

	if (_totalLen > maxLength)
		throw BadRLPFormat();

	/* Add children */
	if (prefix >= 192)
		parseItems();
}


void RLP::parseDataLength(size_t dataLengthSize)
{
	_dataLen = 0;
	for (size_t i = 0; i < dataLengthSize; ++i) {
		_dataLen *= 256;
		_dataLen += _contents[_prefixOff + 1 + i];

        if (_dataLen > _contents.size() - _prefixOff + dataLengthSize + 1) {
            throw runtime_error("DataLen too big");
        }
	}
}


void RLP::parseItems()
{
	size_t remLen = _dataLen;
	size_t parseOffset = _dataOff;
	while (remLen > 0) {
		_items.push_back(RLP{_contents, parseOffset, remLen});
		remLen -= _items.back().totalLength();
		parseOffset += _items.back().totalLength();
	}
}

vector<uint8_t> RLP::serialize(const vector<RLPField> & dataFields)
{
	vector<uint8_t> items;

	for (auto f : dataFields) {
		if (!f.isSerialized) {
			if (f.bytes.size() == 0) {
				items.push_back(128u);

			} else if (f.bytes.size() == 1 && f.bytes[0] < 128u) {
				items.push_back(f.bytes[0]);

			} else if (f.bytes.size() < 56) {
				items.push_back(128u + f.bytes.size());
				size_t tmp = items.size();
				items.resize(tmp + f.bytes.size());
				memcpy(items.data() + tmp, f.bytes.data(), f.bytes.size());

			} else {
				vector<uint8_t> s;
				size_t tmp = f.bytes.size();
				while (tmp > 0) {
					s.push_back(tmp % 256);
					tmp /= 256;
				}

				items.push_back(183u + s.size());

				while (!s.empty()) {
					items.push_back(s.back());
					s.pop_back();
				}

				tmp = items.size();
				items.resize(tmp + f.bytes.size());
				memcpy(items.data() + tmp, f.bytes.data(), f.bytes.size());

			}

		} else {

			if (f.bytes.size() == 0) {
				items.push_back(192u);

			} else {
				size_t tmp = items.size();
				items.resize(tmp + f.bytes.size());
				memcpy(items.data() + tmp, f.bytes.data(), f.bytes.size());

			}
		}
	}

	if (dataFields.size() == 1 && !dataFields[0].isSerialized)
		return items;

	vector<uint8_t> prefix;

	if (items.size() < 56) {
		prefix.push_back(192u + items.size());
	} else {
		vector<uint8_t> s;
		size_t tmp = items.size();
		while (tmp > 0) {
			s.push_back(tmp % 256);
			tmp /= 256;
		}

		prefix.push_back(247u + s.size());

		while (!s.empty()) {
			prefix.push_back(s.back());
			s.pop_back();
		}
	}

	vector<uint8_t> result;
	result.resize(prefix.size() + items.size());
	memcpy(result.data(), prefix.data(), prefix.size());
	memcpy(result.data() + prefix.size(), items.data(), items.size());

	return result;
}


vector<uint8_t> numberToVector(size_t input) {
    vector<uint8_t> result;
    vector<uint8_t> tmp; //will be filled with bytes and later reversed into result vector
    while (input > 0) {
            tmp.push_back(input % 256);
            input /= 256;
    }
    while (!tmp.empty()) {
            result.push_back(tmp.back());
            tmp.pop_back();
    }
    return result;
}
