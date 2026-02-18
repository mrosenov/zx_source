#include "indexmaker.hpp"
#include "indexdb.hpp"
#include "logcommon/util.hpp"
#include "logcommon/config.hpp"
#include "logcommon/filenamerule.hpp"
#include "logcommon/recordreader.hpp"

namespace LOG
{
	void IndexMaker::add( Index * idx )
        {
                idx_mmap.insert( std::make_pair(idx->get_type(), new _Index(idx)) );
        }

	void IndexMaker::close( )
	{
		delete this;
	}

        void IndexMaker::make( )
        {
		init();
		if( src_fileset.empty() )
		{
			std::cout << "logfile set is empty!" << std::endl;
			return;
		}
		dump_src_fileset();
		int num = 0;
		////////////////////////////////
		//
		// 依次对搜集的文件建立索引
		//
		////////////////////////////////
                for( fileset_t::iterator fileset_it = src_fileset.begin(); fileset_it != src_fileset.end(); ++fileset_it )
		{
			std::cout << "Create index file ... " << ++num << std::endl;
			mmap_t sub_idx_mmap;

			/////////////////////////////////
			//
			// get_exist_idx_map for rebuild
			//
			/////////////////////////////////
			get_exist_idx_map( *fileset_it, sub_idx_mmap );
			if( sub_idx_mmap.size() <= 0 )
			{
				std::cout <<  "No index file to create:" << *fileset_it << std::endl;
				continue;
			}

			open_db( *fileset_it, sub_idx_mmap );
			std::auto_ptr< RecordReader > recordreader( new RecordReader(*fileset_it, false /*no wait*/) );
			IndexInfo ii;
			GNET::Marshal::OctetsStream data;
			RecordHead rh;
			while( Env::Inst().running )
			{
				if ( !recordreader->read( data, rh, &ii ))
					break;
				std::pair< iterator, iterator > range = sub_idx_mmap.equal_range( rh.type );
				GNET::Protocol *p = NULL;
				if( range.first != range.second )
				{
					p = range.first->second->index->createProtocol( data );
					for ( ; range.first != range.second; ++range.first )
					{
						keyvalue_t keyvalue = range.first->second->index->extractkeyvalue( p );
						range.first->second->db->insert( keyvalue, ii );
					}
					if( p )
						p->Destroy();
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 对maker是否rebuild进行控制
	// 对判断当前程序是否重复运行进行控制
	//
	///////////////////////////////////////////////////////////////////////////////////////////
	void IndexMaker::get_exist_idx_map( const std::string &src_file, mmap_t &sub_idx_mmap ) const
	{
		for( mmap_t::const_iterator it = idx_mmap.begin(); it != idx_mmap.end(); ++it )
		{
			std::string db_file = FilenameRule::log2idx(src_file, logidxdir, (*it).second->index->get_type(), 
					(*it).second->index->get_keyname() );
			bool acce = (access( db_file.c_str(), R_OK ) == 0);

			////////////////////////////////////////////////
			//
			// 控制只有一个进程打开db_file
			//
			////////////////////////////////////////////////
			if( acce && checkfile_opened_by_othermaker(db_file) ) 
				throw LOG::Exception( "The db_file is opened: " + db_file ); 

			///////////////////////////////////////////////////////////////
			//
			// 如果是tail状态,df_file件存在,直接删除,不考虑rebuild
			//
			///////////////////////////////////////////////////////////////
			if( ( acce && Env::Inst().tail ) || ( acce && (*it).second->index->is_rebuild() ) )
				remove( db_file.c_str() );
			else if( acce )
			{
				std::cout <<  "Ignore exist file:" <<  db_file << std::endl;
				continue;
			}
			sub_idx_mmap.insert( *it );
		}
	}


	void IndexMaker::open_db( const std::string &src_file, const mmap_t &sub_idx_mmap )
	{
		for( mmap_t::const_iterator mmap_it = sub_idx_mmap.begin(); mmap_it != sub_idx_mmap.end(); ++mmap_it )
		{
			std::string db_file = FilenameRule::log2idx( src_file, 
						logidxdir, 
						(*mmap_it).second->index->get_type(), 
						(*mmap_it).second->index->get_keyname() );

			(*mmap_it).second->open_index_db( db_file );
			std::cout << "Source File :  " << src_file << std::endl;
			std::cout << "==> " << db_file << std::endl;
		}
	}

	IndexMaker::IndexMaker( const std::string &_multi_file ):multi_file(_multi_file) { }

        IndexMaker::~IndexMaker( )
        {
                for( mmap_t::iterator mmap_it = idx_mmap.begin(); mmap_it != idx_mmap.end(); ++ mmap_it )
                {
                        delete (*mmap_it).second;
                }
        }

	void IndexMaker::dump_src_fileset( )
        {
                std::cout << "--------------- log fileset  size = " << src_fileset.size() << "--------------"  << std::endl;
                for( fileset_t::iterator it = src_fileset.begin(); it != src_fileset.end(); ++it )
                        std::cout << (*it) << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;
        }

	////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 如果没有指定建索引的文件, 则由配置决定哪些文件需要建索引
	// 最后对文件集合过滤下不合法的文件
	//	
	////////////////////////////////////////////////////////////////////////////////////////////
	void IndexMaker::init( )
	{
		logdir = Config2::GetInstance()->GetRecordDir();
		logidxdir = Config2::GetInstance()->GetParseDir();
		logrotatedir = Config2::GetInstance()->GetRotateDir();

		if( ! multi_file.empty() )
			FilenameRule::get_file_or_dir_fileset( multi_file, src_fileset );
		else
		{
			type_file_map_t type_file_map = Config2::GetInstance()->get_type_file_map();
			for( mmap_t::iterator it = idx_mmap.begin(); it != idx_mmap.end(); ++it )
			{
				type_file_map_t::iterator tmit = type_file_map.find( it->second->index->get_type() );
				if( tmit != type_file_map.end() )
				{
					fileset_t::iterator fsit = tmit->second.begin(); 
					for( ;fsit != tmit->second.end(); ++fsit)
						lognameset.insert( FilenameRule::get_logdir_logname( *fsit ) );
				}
			}
			std::string tmpstr =  Config2::GetInstance()->GetDefaultFile();
			if( ! tmpstr.empty() )
				lognameset.insert( FilenameRule::get_logdir_logname( tmpstr ) );
			tmpstr =  Config2::GetInstance()->GetAnyFile();
			if( ! tmpstr.empty() )
				lognameset.insert( FilenameRule::get_logdir_logname( tmpstr ) );
			logname2fileset( lognameset );
		}
		/*
		for( fileset_t::iterator it = src_fileset.begin(); it != src_fileset.end(); ++it )
		{
			if( ! FilenameRule::check_logrotatedir_file( *it ) )	
				src_fileset.erase( it );
		}
		*/
	}

	/////////////////////////////////////////////////////////////////////////
	//
	// 只用于批量建索引,不考虑logdir目录	
	//
	/////////////////////////////////////////////////////////////////////////
	void IndexMaker::logname2fileset( const lognameset_t &lognameset )
	{
		std::string logrotatedir = Config2::GetInstance()->GetRotateDir();
		ReadDir readdir( logrotatedir);
		fileset_t tempset = readdir.getfileset();
                fileset_t::const_iterator it = tempset.begin();
		for(; it != tempset.end(); ++it)
		{
			std::string fullpath = logrotatedir + *it;
			if( lognameset.find( FilenameRule::get_logrotatedir_logname( fullpath )) != lognameset.end() 
				&& FilenameRule::check_logrotatedir_file( fullpath )
				) 
				src_fileset.insert( fullpath );
		}
	}

	bool IndexMaker::checkfile_opened_by_othermaker( const std::string &db_file ) const
	{
		int fd = open( db_file.c_str(), O_CREAT | O_RDWR, FILE_MODE );
		if( fd == -1 )
			Error( "open file error" + db_file );
		if( write_lock(fd, 0, SEEK_SET, 0) < 0 )
		{
			if( errno == EACCES || errno == EAGAIN )
				Error("program is already running.");
			else
				Error("write_lock error.");
		}
		::close(fd);
		return false;
	}

}//end namespace LOG
