#pragma once

#include "spdlog/details/pattern_formatter.h"

#include "krpc/krpc/stream.hpp"

namespace spdlog {

namespace details {

class krpc_ut_formatter : public flag_formatter
{
public:
	explicit krpc_ut_formatter(padding_info padinfo, std::shared_ptr<krpc::Stream<double>> ut)
		: flag_formatter(padinfo)
		, ut_(ut)
	{}

	void format(const details::log_msg &, const std::tm &, fmt::memory_buffer &dest) override
	{
		const size_t field_size = 22;
		scoped_pad p(field_size, padinfo_, dest);
		fmt::format_to(dest, "UT: {:19.3f}", ut_->operator()());
	}

protected:
	std::shared_ptr<krpc::Stream<double>> ut_;
};

class krpc_met_formatter : public flag_formatter
{
public:
	explicit krpc_met_formatter(padding_info padinfo, std::shared_ptr<krpc::Stream<double>> met)
		: flag_formatter(padinfo)
		, met_(met)
	{}

	void format(const details::log_msg &, const std::tm &, fmt::memory_buffer &dest) override
	{
		const size_t field_size = 22;
		scoped_pad p(field_size, padinfo_, dest);
		fmt::format_to(dest, "MET:{:18.3f}", met_->operator()());
	}

protected:
	std::shared_ptr<krpc::Stream<double>> met_;
};

}

class krpc_pattern_formatter final : public formatter
{
public:
    explicit krpc_pattern_formatter(
        std::string pattern, 
	std::shared_ptr<krpc::Stream<double>> ut = NULL, 
	std::shared_ptr<krpc::Stream<double>> met = NULL, 
	pattern_time_type time_type = pattern_time_type::local, 
	std::string eol = spdlog::details::os::default_eol
    )
        : pattern_(std::move(pattern))
        , eol_(std::move(eol))
        , pattern_time_type_(time_type)
        , last_log_secs_(0)
	, ut_(ut)
	, met_(met)
    {
        std::memset(&cached_tm_, 0, sizeof(cached_tm_));
        compile_pattern_(pattern_);
    }

    // use by default full formatter for if pattern is not given
    explicit krpc_pattern_formatter(pattern_time_type time_type = pattern_time_type::local, std::string eol = spdlog::details::os::default_eol)
        : pattern_()
        , eol_(std::move(eol))
        , pattern_time_type_(time_type)
        , last_log_secs_(0)
    {
        std::memset(&cached_tm_, 0, sizeof(cached_tm_));
        formatters_.push_back(details::make_unique<details::full_formatter>(details::padding_info{}));
    }

    krpc_pattern_formatter(const krpc_pattern_formatter &other) = delete;
    krpc_pattern_formatter &operator=(const krpc_pattern_formatter &other) = delete;

    std::unique_ptr<formatter> clone() const override
    {
        return details::make_unique<krpc_pattern_formatter>(pattern_, ut_, met_, pattern_time_type_, eol_);
    }

    void format(const details::log_msg &msg, fmt::memory_buffer &dest) override
    {
#ifndef SPDLOG_NO_DATETIME
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(msg.time.time_since_epoch());
        if (secs != last_log_secs_)
        {
            cached_tm_ = get_time_(msg);
            last_log_secs_ = secs;
        }
#endif
        for (auto &f : formatters_)
        {
            f->format(msg, cached_tm_, dest);
        }
        // write eol
        details::fmt_helper::append_string_view(eol_, dest);
    }

private:
    std::string pattern_;
    std::string eol_;
    pattern_time_type pattern_time_type_;
    std::tm cached_tm_;
    std::chrono::seconds last_log_secs_;

    std::vector<std::unique_ptr<details::flag_formatter>> formatters_;

    std::shared_ptr<krpc::Stream<double>> ut_;
    std::shared_ptr<krpc::Stream<double>> met_;

    std::tm get_time_(const details::log_msg &msg)
    {
        if (pattern_time_type_ == pattern_time_type::local)
        {
            return details::os::localtime(log_clock::to_time_t(msg.time));
        }
        return details::os::gmtime(log_clock::to_time_t(msg.time));
    }

    void handle_flag_(char flag, details::padding_info padding)
    {
        switch (flag)
        {
        // logger name
        case 'n':
            formatters_.push_back(details::make_unique<details::name_formatter>(padding));
            break;

        case 'l':
            formatters_.push_back(details::make_unique<details::level_formatter>(padding));
            break;

        case 'L':
            formatters_.push_back(details::make_unique<details::short_level_formatter>(padding));
            break;

        case ('t'):
            formatters_.push_back(details::make_unique<details::t_formatter>(padding));
            break;

        case ('v'):
            formatters_.push_back(details::make_unique<details::v_formatter>(padding));
            break;

        case ('a'):
            formatters_.push_back(details::make_unique<details::a_formatter>(padding));
            break;

        case ('A'):
            formatters_.push_back(details::make_unique<details::A_formatter>(padding));
            break;

        case ('b'):
        case ('h'):
            formatters_.push_back(details::make_unique<details::b_formatter>(padding));
            break;

        case ('B'):
            formatters_.push_back(details::make_unique<details::B_formatter>(padding));
            break;
        case ('c'):
            formatters_.push_back(details::make_unique<details::c_formatter>(padding));
            break;

        case ('C'):
            formatters_.push_back(details::make_unique<details::C_formatter>(padding));
            break;

        case ('Y'):
            formatters_.push_back(details::make_unique<details::Y_formatter>(padding));
            break;

        case ('D'):
        case ('x'):
            formatters_.push_back(details::make_unique<details::D_formatter>(padding));
            break;

        case ('m'):
            formatters_.push_back(details::make_unique<details::m_formatter>(padding));
            break;

        case ('d'):
            formatters_.push_back(details::make_unique<details::d_formatter>(padding));
            break;

        case ('H'):
            formatters_.push_back(details::make_unique<details::H_formatter>(padding));
            break;

        case ('I'):
            formatters_.push_back(details::make_unique<details::I_formatter>(padding));
            break;

        case ('M'):
            formatters_.push_back(details::make_unique<details::M_formatter>(padding));
            break;

        case ('S'):
            formatters_.push_back(details::make_unique<details::S_formatter>(padding));
            break;

        case ('e'):
            formatters_.push_back(details::make_unique<details::e_formatter>(padding));
            break;

        case ('f'):
            formatters_.push_back(details::make_unique<details::f_formatter>(padding));
            break;
        case ('F'):
            formatters_.push_back(details::make_unique<details::F_formatter>(padding));
            break;

        case ('E'):
            formatters_.push_back(details::make_unique<details::E_formatter>(padding));
            break;

        case ('p'):
            formatters_.push_back(details::make_unique<details::p_formatter>(padding));
            break;

        case ('r'):
            formatters_.push_back(details::make_unique<details::r_formatter>(padding));
            break;

        case ('R'):
            formatters_.push_back(details::make_unique<details::R_formatter>(padding));
            break;

        case ('T'):
        case ('X'):
            formatters_.push_back(details::make_unique<details::T_formatter>(padding));
            break;

        case ('z'):
            formatters_.push_back(details::make_unique<details::z_formatter>(padding));
            break;

        case ('+'):
            formatters_.push_back(details::make_unique<details::full_formatter>(padding));
            break;

        case ('P'):
            formatters_.push_back(details::make_unique<details::pid_formatter>(padding));
            break;
#ifdef SPDLOG_ENABLE_MESSAGE_COUNTER
        case ('i'):
            formatters_.push_back(details::make_unique<details::i_formatter>(padding));
            break;
#endif
        case ('^'):
            formatters_.push_back(details::make_unique<details::color_start_formatter>(padding));
            break;

        case ('$'):
            formatters_.push_back(details::make_unique<details::color_stop_formatter>(padding));
            break;

        case ('%'):
            formatters_.push_back(details::make_unique<details::ch_formatter>('%'));
            break;
	
	case ('U'):
	    if (ut_)
	    	formatters_.push_back(details::make_unique<details::krpc_ut_formatter>(padding, ut_));
	    break;

	case ('Z'):
	    if (met_)
	    	formatters_.push_back(details::make_unique<details::krpc_met_formatter>(padding, met_));
	    break;

        default: // Unknown flag appears as is
            auto unknown_flag = details::make_unique<details::aggregate_formatter>();
            unknown_flag->add_ch('%');
            unknown_flag->add_ch(flag);
            formatters_.push_back((std::move(unknown_flag)));
            break;
        }
    }

    // Extract given pad spec (e.g. %8X)
    // Advance the given it pass the end of the padding spec found (if any)
    // Return padding.
    details::padding_info handle_padspec_(std::string::const_iterator &it, std::string::const_iterator end)
    {
        using details::padding_info;
        using details::scoped_pad;
        const size_t max_width = 128;
        if (it == end)
        {
            return padding_info{};
        }

        padding_info::pad_side side;
        switch (*it)
        {
        case '-':
            side = padding_info::right;
            ++it;
            break;
        case '=':
            side = padding_info::center;
            ++it;
            break;
        default:
            side = details::padding_info::left;
            break;
        }

        if (it == end || !std::isdigit(static_cast<unsigned char>(*it)))
        {
            return padding_info{0, side};
        }

        auto width = static_cast<size_t>(*it - '0');
        for (++it; it != end && std::isdigit(static_cast<unsigned char>(*it)); ++it)
        {
            auto digit = static_cast<size_t>(*it - '0');
            width = width * 10 + digit;
        }
        return details::padding_info{std::min(width, max_width), side};
    }

    void compile_pattern_(const std::string &pattern)
    {
        auto end = pattern.end();
        std::unique_ptr<details::aggregate_formatter> user_chars;
        formatters_.clear();
        for (auto it = pattern.begin(); it != end; ++it)
        {
            if (*it == '%')
            {
                if (user_chars) // append user chars found so far
                {
                    formatters_.push_back(std::move(user_chars));
                }

                auto padding = handle_padspec_(++it, end);

                if (it != end)
                {
                    handle_flag_(*it, padding);
                }
                else
                {
                    break;
                }
            }
            else // chars not following the % sign should be displayed as is
            {
                if (!user_chars)
                {
                    user_chars = details::make_unique<details::aggregate_formatter>();
                }
                user_chars->add_ch(*it);
            }
        }
        if (user_chars) // append raw chars found so far
        {
            formatters_.push_back(std::move(user_chars));
        }
    }
};
} // namespace spdlog
