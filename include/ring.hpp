#ifndef RING_HPP
#define RING_HPP

#ifndef MAPSIZE
#define MAPSIZE (256)
#endif

#ifndef IVMAXSIZE
#define IVMAXSIZE (1024)
#endif

class Ring
{
	public:
		/**
		 * Creates a Ring.
		 * @param[in] c password
		 * @param[in] length length of \p c
		 * @param[in] salt salt (may be public)
		 * @param[in] saltLength length of salt (salt)
		 * In encodemode this should be set to a random value.
		 * @param[in] mutationInterval number of operations until ring is mutated (may be public)
		 * If you want no mutation set it to 0.
		 * @warning Decreasing mutationInterval too much may cause low performance!
		 * @pre \p key is allocated for length characters
		 * @pre \p length > 0
		 * @pre \p length <= MAPSIZE
		 * @pre \p salt is allocated for \p saltLength characters
		 * @pre \p saltLength > 0
		 * @pre \p saltLength <= IVMAXSIZE
		 */
		Ring(const char* key, unsigned int length, const char* salt, unsigned int saltLength, unsigned int mutationInterval);
		/**
		 * Deletes a Ring.
		 */
		virtual ~Ring();
		/**
		 * Reinitialises a Ring.
		 * @note This is needed between 2 independent encode/decode operations (e.g. 2 files).
		 * @post Ring in post constructed status
		 */
		void reinit();
		/**
		 * Encodes an array of characters inplace.
		 * @param[in,out] c array of characters to encode
		 * @pre \p c is allocated for \p length characters
		 * @pre length > 0
		 * @post \p c contains encoded characters
		 */
		void encode(char* c, unsigned int length);
		/**
		 * Decodes an array of characters inplace.
		 * @param[in,out] c array of characters to decode
		 * @pre \p c is allocated for \p length characters
		 * @pre \p length > 0
		 * @post \p c contains decoded characters
		 */
		void decode(char* c, unsigned int length);
	protected:
		unsigned char last;
		/**
		 * Mutates the map independent from the key.
		 * This method is used to mutate the ring after mutationInterval operations.
		 */
		void shuffle();
		unsigned int mutationInterval;
		unsigned int operationsSinceMutation;
		/**
		 * Mutates the map depending on the key.
		 * This method is used to initialise the ring.
		 * @deprecated This method is only in use for initialisation of the ring. All other mutations are done with shuffle() because this method needs too much time.
		 */
		void mutate();
		/**
		 * Encodes a single character.
		 * @param[in] c character to encode
		 * @return encoded character
		 */
		unsigned char encode(unsigned char c);
		/**
		 * Decodes a single character.
		 * @param[in] c character to decode
		 * @return decoded character
		 */
		unsigned char decode(unsigned char c);
		unsigned char map[MAPSIZE];
		unsigned char decodeMap[MAPSIZE];
		unsigned char salt[IVMAXSIZE];
		unsigned int saltLength;
		unsigned char initSalt[IVMAXSIZE];
		unsigned int actualSalt;
		unsigned char pw[MAPSIZE];
		unsigned int pwLength;
	private:
};

#endif
