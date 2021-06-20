class LogSplit {
public:
	explicit LogSplit(const std::string &log_name, uint64_t limit_size) :
		m_log_name(log_name), m_split_cnt(0), m_limit_size(limit_size),
		m_curr_size(0), m_curr_fp(nullptr) {
		split();
	}
	
	~LogSplit() = default;
	
	void add_size(uint32_t size) {
		m_curr_size += size;
		if (m_curr_size >= m_limit_size) {
			split();
			m_curr_size = 0;
		}
	}
	
	void split() {
		time_t curr_time = time(nullptr);
		struct tm tm_ds;
		auto ret = localtime_r(&curr_time, &tm_ds);
		if (ret != 0) {
			printf("localtime_r log split failed\n");
			return;
		}
		char log_name[1024];
		snprintf(log_name, 1024, "../log/%s.%04d%02d%02d_%02d%02d%02d.log.%d",
			m_log_name.c_str(), tm_ds.tm_year + 1900, tm_ds.tm_mon + 1, tm_ds.tm_mday,
			tm_ds.tm_hour, tm_ds.tm_min, tm_ds.tm_sec, ++m_split_cnt);
		if (m_curr_fp != nullptr) {
			fclose(m_curr_fp);
		}
		m_curr_fp = fopen(log_name, "w");
		std::string symlink_name = log_name + ".log";
		unlink(symlink_name.c_str());
		symlink(log_name, symlink_name.c_str());
	}
	
private:
	std::string m_log_name;
	uint64_t m_limit_size;
	uint64_t m_curr_size;
	uint32_t m_split_cnt;
	FILE *m_curr_fp;
};
