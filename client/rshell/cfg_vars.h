
#ifndef ___CFG_VARS_H___
#define ___CFG_VARS_H___


#include "types.h"
#include "cfg.h"


// �������
extern CContent g_content;


// ����� ���������
extern TPATH wait_server_path;
extern int wait_server_timeout;
extern BOOL only_one_cpu;
extern int shutdown_action;

// ������� ����������
extern int fastexit_idle_timeout;
extern BOOL fastexit_use_keyboard;
extern BOOL fastexit_use_fixed_pwd;
extern TSTRING fastexit_fixed_pwd_md5;
extern BOOL fastexit_use_flash;
extern TLONGSTRING fastexit_flash_list;

// ���������: ���
extern int def_sheet_color;
extern int curr_theme2d;
extern TPATH curr_desktop_theme;
extern BOOL dont_show_theme_errors;
extern TSTRING html_status_text1;
extern TLONGSTRING html_status_text2;
extern BOOL icons_winstyle;
extern BOOL do_icon_highlight;
extern int ubericon_effect;
extern BOOL use_desk_shader;
extern BOOL high_quality_bg_video;
extern BOOL dont_show_empty_icons;
extern BOOL det_empty_icons_by_icon_path;
extern BOOL dont_show_icon_names;
extern BOOL use_system_icon_spacing;
extern int icon_spacing_w;
extern int icon_spacing_h;
extern int icon_size_h;
extern int icon_size_w;
extern int close_sheet_idle;
extern int min_grouped_windows;
extern BOOL disable_desktop_composition;
extern TCFGLIST2 user_tools;
extern BOOL sheet_init_maximize;
extern TSTRING startup_sheet_name;

// ���������: ����
extern BOOL winkey_enable;
extern BOOL vip_in_menu;
extern BOOL show_book_in_menu;
extern BOOL monitor_off_in_menu;
extern BOOL logoff_in_menu;
extern BOOL reboot_in_menu;
extern BOOL shutdown_in_menu;
extern BOOL gc_info_in_menu;
extern BOOL mycomp_in_menu;
extern BOOL calladmin_in_menu;

// ������������: ��������� �������
extern BOOL sysrestrict00;
extern BOOL sysrestrict01;
extern BOOL sysrestrict02;
extern BOOL sysrestrict03;
extern BOOL sysrestrict04;
extern BOOL sysrestrict05;
extern BOOL sysrestrict06;
extern BOOL sysrestrict07;
extern BOOL sysrestrict08;
extern BOOL sysrestrict09;
extern BOOL sysrestrict10;
extern BOOL sysrestrict11;
extern BOOL sysrestrict12;
extern BOOL sysrestrict13;
extern BOOL sysrestrict14;
extern BOOL sysrestrict15;
extern BOOL sysrestrict16;
extern BOOL sysrestrict17;
extern BOOL sysrestrict18;
extern BOOL sysrestrict19;
extern BOOL sysrestrict20;
extern BOOL sysrestrict21;
extern BOOL sysrestrict22;
extern BOOL sysrestrict23;
extern BOOL sysrestrict24;
extern BOOL sysrestrict25;
extern BOOL sysrestrict26;
extern BOOL sysrestrict27;
extern BOOL sysrestrict28;
extern BOOL sysrestrict29;

// ������������: �������� �������
extern BOOL restrict_copyhook;
extern BOOL restrict_shellexechook;
extern TSTRING protected_protos;
extern TLONGSTRING allowed_ext;
extern int disks_hidden;
extern int disks_disabled;
extern BOOL restrict_file_dialogs;
extern BOOL allow_newfolder_opensave;
extern TSTRING apps_opensave_prohibited;
extern BOOL allow_use_flash;
extern BOOL allow_use_diskette;
extern BOOL allow_use_cdrom;
extern TPATH net_flash;
extern TPATH net_diskette;
extern TPATH net_cdrom;
extern BOOL allow_flash_stat;
extern BOOL allow_dvd_stat;
extern int clean_user_folder;
extern TPATH user_folder_base;
extern TSTRING uf_format;
extern BOOL allow_run_from_folder_shortcuts;

// ������������: ����, �������� � ��.
extern TCFGLIST2 disable_windows;
extern BOOL close_not_active_windows;
extern TCFGLIST1 disallow_run;
extern TCFGLIST1 allow_run;
extern BOOL disallow_power_keys;
extern BOOL disable_input_when_monitor_off;
extern BOOL safe_console;
extern BOOL safe_winamp;
extern BOOL safe_mplayerc;
extern BOOL safe_powerdvd;
extern BOOL safe_torrent;
extern BOOL safe_garena;

// ������������: CTRL+ALT+DEL
extern BOOL use_cad_catcher;
extern BOOL cad_taskman;
extern BOOL cad_killall;
extern BOOL cad_gcinfo;
extern BOOL cad_reboot;
extern BOOL cad_shutdown;
extern BOOL cad_monitoroff;
extern BOOL cad_logoff;

// ������������: ����
extern int max_vis_tray_icons;
extern BOOL safe_tray;
extern TCFGLIST1 safe_tray_icons;
extern TCFGLIST1 hidden_tray_icons;

// ������ �������
extern BOOL allow_run_only_one;
extern BOOL protect_bodytools_when_nosql;
extern BOOL protect_run_when_nosql;
extern BOOL protect_in_safe_mode;
extern BOOL protect_run_at_startup;
extern BOOL use_blocker;
extern TPATH blocker_file;
extern int ssaver_idle;
extern TPATH alcohol_path;
extern TPATH daemon_path;
extern TPATH daemon_pro_path;
extern TCFGLIST2 lic_manager;

// ������ �������: ����������
extern BOOL stat_enable;
extern int clear_stat_interval;
extern BOOL do_web_stat;
extern TSTRING stat_excl;

// ������������
extern BOOL allow_printer_control;
extern BOOL allow_hwident_ibutton;

// �����: ��������������� ��� VIP
extern BOOL redirect_sys_folders;
extern BOOL redirect_personal;
extern BOOL redirect_appdata;
extern BOOL redirect_localappdata;
extern TPATH personal_path;
extern TPATH vip_basefolder;
extern int vip_folder_limit;
extern BOOL force_viplogin_from_api;

// ���������
extern TCFGLIST2 addon_folders;
extern BOOL allow_save_to_addon_folders;
extern BOOL allow_drag_anywhere;
extern BOOL allow_write_to_removable;
extern TCFGLIST2 menu_ext;
extern TCFGLIST2 menu_ext_rev;
extern TPATH winrar_path;
extern BOOL delete_to_recycle;
extern BOOL disallow_copy_from_lnkfolder;
extern BOOL show_hiddens_in_bodyexpl;

// ������� IE: �����
extern BOOL use_bodytool_ie;
extern BOOL disallow_sites;
extern TCFGLIST1 disallowed_sites;
extern TCFGLIST1 allowed_sites;
extern TCFGLIST1 redirected_urls;
extern TSTRING redirection_url;
extern TSTRING ie_local_res;
extern TSTRING ie_disallowed_protos;
extern TSTRING safe_ie_exts;
extern TSTRING safe_ie_protos;
extern TPATH fav_path;
extern BOOL disallow_add2fav;
extern TSTRING ie_open_with_mp;
extern TSTRING ie_open_with_ext;
extern BOOL wb_flash_disable;
extern BOOL allow_ie_print;
extern int max_ie_windows;
extern BOOL ie_clean_history;
extern BOOL rus2eng_wb;
extern BOOL close_ie_when_nosheet;
extern BOOL ftp_enable;
extern BOOL protect_run_in_ie;
extern BOOL wb_search_bars;
extern BOOL disable_view_html;
extern BOOL ie_use_sett;
extern TSTRING ie_sett_proxy;
extern TSTRING ie_sett_port;
extern TSTRING ie_sett_autoconfig;
extern TSTRING ie_home_page;
extern TSTRING bodywb_caption;

// ������� IE: ���������
extern BOOL use_std_downloader;
extern TSTRING allowed_download_types;
extern BOOL use_allowed_download_types;
extern TSTRING std_downloader_sites;
extern int max_download_windows;
extern int max_download_size;
extern int download_speed_limit;
extern TSTRING download_autorun;
extern BOOL dont_show_download_speed;
extern BOOL allow_run_downloaded;

// �������: MediaPlayer
extern BOOL use_bodytool_mp;
extern TSTRING safe_mp_exts;
extern TSTRING safe_mp_exts_winamp;
extern TSTRING safe_mp_protos;
extern TPATH alternate_mp;

// �������: ��������� ������� / bluetooth
extern TCFGLIST2 mobile_content;
extern TSTRING mobile_files_audio;
extern TSTRING mobile_files_video;
extern TSTRING mobile_files_pictures;
extern BOOL mobile_bodyexpl_integration;
extern BOOL allow_bt_stat;
extern BOOL bt_integration;
extern TPATH net_bt_path;

// �������: ������ ������
extern BOOL allow_burn_stat;
extern BOOL burn_integration;
extern TSTRING law_protected_files;
extern TPATH net_burn_path;
extern TPATH on_burn_complete;

// �������: Mail
extern BOOL allow_mail_stat;
extern BOOL bodymail_integration;
extern TSTRING bodymail_smtp;
extern TSTRING bodymail_user;
extern TSTRING bodymail_password;
extern int bodymail_port;
extern BOOL bodymail_hardcoded;
extern TSTRING bodymail_from_name;
extern TSTRING bodymail_from_address;
extern TSTRING bodymail_footer;
extern TSTRING bodymail_to;

// �������: ��������� �����
extern TCFGLIST1 hide_tm_programs;
extern BOOL safe_taskmgr;
extern BOOL safe_taskmgr2;
extern BOOL safe_taskmgr3;
extern BOOL kill_hidden_tasks;

// �������: ������
extern BOOL use_bodytool_office;
extern BOOL protect_run_in_office;
extern BOOL ext_office_print;
extern BOOL show_office_menu;
extern BOOL use_bodytool_notepad;
extern TSTRING safe_notepad_exts;
extern BOOL use_bodytool_imgview;
extern BOOL use_bodytool_pdf;
extern BOOL show_pdf_panel;
extern BOOL use_bodytool_swf;
extern TSTRING inject_scan;
extern BOOL allow_scan_stat;
extern BOOL tray_indic;
extern BOOL tray_minimize_all;
extern BOOL tray_mixer;
extern BOOL tray_microphone;
extern BOOL allow_photocam;

// ������������: ��������
extern TCFGLIST1 delete_folders;
extern BOOL clean_temp_dir;
extern BOOL clean_ie_dir;
extern BOOL clean_cookies;
extern BOOL clear_recycle_bin;
extern BOOL clear_print_spooler;

// ������������: ����������
extern BOOL autoplay_cda;
extern TPATH autoplay_cda_cmd;
extern BOOL autoplay_dvd;
extern TPATH autoplay_dvd_cmd;
extern BOOL autoplay_cdr;
extern TPATH autoplay_cdr_cmd;
extern BOOL autoplay_flash;
extern TPATH autoplay_flash_cmd;

// ������������: ������������
extern TSTRING welcome_path;
extern TCFGLIST2 autorun_items;
extern BOOL disable_autorun;
extern BOOL show_la_at_startup;
extern TPATH la_startup_path;

// ������������: volume control
extern int maxvol_master;  // 0-100
extern int maxvol_wave;    // 0-100
extern int minvol_master;  // 0-100
extern int minvol_wave;    // 0-100
extern BOOL maxvol_enable;

// ������������: mouse control
extern int adj_mouse_speed;
extern int adj_mouse_acc;
extern BOOL allow_mouse_adj;

// ������������: scandisk
extern BOOL do_scandisk;
extern int scandisk_hours;
extern int scandisk_disks;

// ������������: display mode
extern BOOL restore_dm_at_startup;
extern int def_vmode_width;
extern int def_vmode_height;
extern int def_vmode_bpp;
extern int display_freq;

// ������������: ������
extern int turn_off_idle;
extern BOOL use_logoff_in_turn_off_idle;
extern TPATH client_restore;
extern BOOL use_time_limitation;
extern TSTRING time_limitation_intervals;
extern int time_limitation_action;


//////////////////////////////////////////////////
// comp_specific vars
//////////////////////////////////////////////////

// rollback
extern BOOL use_rollback;
extern int rollback_disks;
extern TCFGLIST1 rollback_excl;
extern BOOL used_another_rollback;



#endif
