#ifndef __LOG_INDEXMAKER_HPP__
#define __LOG_INDEXMAKER_HPP__
#include "index.hpp"
#include "indexdb.hpp"
#include "logcommon/config.hpp"
#include "logcommon/appconfig2.hpp"
#include <memory>

namespace LOG
{

class IndexMaker
{
	public:
		void add(Index * idx);
		void make();
		void close();
		IndexMaker(const std::string &multi_file = "");
		~IndexMaker();
	protected:
		struct _Index
		{
			void open_index_db(const std::string &db_file)
			{
				if (db) 
				{
					delete db;
					db = NULL;
				}
				db = new IndexDB(db_file);
			}
			void close_index_db() 
			{ 
				if (db) 
				{
					delete db; 
					db = NULL;
				}
			}

			bool db_is_open() { return db != NULL ? true : false; }


			~_Index()
			{
				index->destroy();
				if( db )
				{
					delete db;
					db = NULL;
				}
			}

			explicit _Index(Index * idx) : index(idx) {  db = NULL; }

			_Index():index(NULL), db(NULL){};

			_Index( const _Index &rhs )
			{
				index = rhs.index;
				db = rhs.db;
			}

			_Index & operator = ( const _Index &rhs )
			{
				if( &rhs == this )
					return *this;
				index = rhs.index;
				db = rhs.db;
				return *this;
			}

			Index * index;
			IndexDB * db;
		};

		typedef std::multimap<type_t, _Index *> mmap_t;
		typedef std::multimap<type_t, _Index *>::iterator iterator;
		typedef std::pair<type_t, _Index *> pair_t;
		lognameset_t lognameset;
		mmap_t idx_mmap;
		std::string logdir, logidxdir, logrotatedir, multi_file;
		fileset_t src_fileset;



		void dump_src_fileset();
		void init();
		void get_exist_idx_map( const std::string &src_file, mmap_t &sub_mmap ) const;
		void open_db(const std::string &src_file, const mmap_t &sub_mmap);
		bool checkfile_opened_by_othermaker(const std::string &db_file) const;
		void logname2fileset( const lognameset_t &lognameset );
};
}
#endif
