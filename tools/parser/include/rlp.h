#ifndef RLP_H
#define RLP_H

#include <cstdint>
#include <exception>
#include <vector>


/**
 * Used for serializing data fields using the Recursive length prefix algorithm
 */
struct RLPField
{
	std::vector<std::uint8_t> bytes; /** vector of bytes to be serialized */
	bool isSerialized; /** indicates whether the vector of bytes represents already serialized data */
};


/**
 * Provides layout of data serialized using the Recursive length prefix algorithm
 */
class RLP
{
	const std::vector<std::uint8_t> & _contents;
	std::size_t _prefixOff;
	std::size_t _dataOff;
	std::size_t _totalLen;
	std::size_t _dataLen;
	std::vector<RLP> _items;

public:
	/**
	 * RLP constructor
	 * Creates RLP object representing the layout of fields in the serialized data
	 * @param contents byte vector of data serialized using RLP
	 */
	explicit RLP(const std::vector<std::uint8_t> & contents);

	/**
	 * RLP constructor
	 * Creates RLP representing the layout of fields in the serialized data
	 * @param contents byte vector of serialized data
	 * @param offest offset to the serialized data
	 * @param maxLength maximum length of the serialized data
	 */
	RLP(const std::vector<std::uint8_t> & contents, std::size_t offset, 
	    std::size_t maxLength);

	/**
	 * @return the length of the data field, including its prefix
	 */
	std::size_t totalLength() const { return _totalLen; }

	/**
	 * @return the length of the data field, excluding its prefix
	 */
	std::size_t dataLength() const { return _dataLen; }

	/**
	 * Accesses a nested serialized item layout
	 * @param index index of the serialized item
	 * @return RLP object representing the layout of a nested item
	 */
	const RLP & operator[](unsigned int index) const {return _items[index];}

	/**
	 * Accesses a nested serialized item layout with boundary checking
	 * @param index index of the serialized item
	 * @return RLP object representing the layout of a nested item
	 */
	const RLP & at(unsigned int index) const {return _items.at(index);}

	/**
	 * Returns the number of nested items
	 * @return the number of nested items
	 */
	unsigned int numItems() const { return _items.size(); }

	/**
	 * Offset to the data field in the underlying byte vector
	 * @return offset
	 */
	std::size_t dataOffset() const { return _dataOff; }

	/**
	 * Offset to the prefix of the data field in the underlying vector
	 * @return offset
	 */
	std::size_t prefixOffset() const { return _prefixOff; }

	/**
	 * Serialize data items using the RLP algorithm
	 * @param dataFields vector data fields to be serialized
	 * @return byte vector of serialized data
	 */
	static std::vector<std::uint8_t> serialize(
		const std::vector<RLPField> & dataFields);

private:
	void parseDataLength(std::size_t dataLengthSize);

	void parseItems();
};


/** Indicates error in parsing serialized data 
 */
class BadRLPFormat : public std::exception
{
public:
	virtual const char* what() const noexcept
	{
		return "Bad RLP format";
	}
};


/**
 * Converts a number to byte vector using big endian and the minimum number of 
 * bytes needed to represent it.
 */
std::vector<std::uint8_t> numberToVector(std::size_t input);

#endif // RLP_H
