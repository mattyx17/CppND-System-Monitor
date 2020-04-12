#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "cpu_time.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


int LinuxParser::ProcessReader(string process_cat) {
  string line;
  string stat_cat;
  int process_count;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> stat_cat;
      if (stat_cat == process_cat) {
        linestream >> process_count;
      }
    }
  }
  return process_count;
}


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


float LinuxParser::MemoryUtilization() {
  string mem_cat;
  double mem_tot, mem_free;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> mem_cat;
      if (mem_cat == "MemTotal:") {
        linestream >> mem_tot;
      } else if (mem_cat == "MemFree:") {
        linestream >> mem_free;
      }
    }
  }
  return (mem_tot - mem_free) / mem_tot;
}


long LinuxParser::UpTime() {
  string line;
  long up_time;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
  }
  return up_time;
}


string LinuxParser::CpuStats(string cpu_name) {
  string stat_cat;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> stat_cat;
      if (stat_cat == cpu_name) {
        return line;
      }
    }
  }
  return line;
}

CpuTime LinuxParser::CpuUtilization() {
  string cpu_stats = LinuxParser::CpuStats("cpu");
  long total_time = LinuxParser::TotalCpuTime(cpu_stats);
  long idle_time = LinuxParser::IdleCpuTime(cpu_stats);
  return CpuTime{total_time, idle_time};
}


long LinuxParser::TotalCpuTime(string cpu_stats) {
  string cpu_name;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice;
  std::istringstream cpustream(cpu_stats);
  cpustream >> cpu_name >> user >> nice >> system >> idle >> iowait;
  cpustream >> irq >> softirq >> steal >> guest >> guestnice;
  user = user - guest;
  nice = nice - guestnice;
  idle = idle + iowait;
  system = system + irq + softirq;
  guest = guest + guestnice;
  return user + nice + idle + system + guest;
}

long LinuxParser::IdleCpuTime(string cpu_stats) {
  string cpu_name;
  long user, nice, system, idle, iowait;
  std::istringstream cpustream(cpu_stats);
  cpustream >> cpu_name >> user >> nice >> system >> idle >> iowait;
  return idle + iowait;
}


long LinuxParser::ActiveTime(int pid) {
  string other_data;
  long utime, stime, cutime, cstime;
  string line;
  string spid = std::to_string(pid) + "/";
  std::ifstream stream(kProcDirectory + spid + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i=0; i < 13; i++) {
      linestream >> other_data;
    }
    linestream >> utime >> stime >> cutime >> cstime;
  }
  return (utime + stime + cutime + cstime) / sysconf(_SC_CLK_TCK);
}




int LinuxParser::TotalProcesses() {
  return LinuxParser::ProcessReader("processes");
}


int LinuxParser::RunningProcesses() {
  return LinuxParser::ProcessReader("procs_running");
}


string LinuxParser::Command(int pid) {
  string cmd;
  string spid = std::to_string(pid) + "/";
  std::ifstream stream(kProcDirectory + spid + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmd);
  }
  return cmd;
}


string LinuxParser::Ram(int pid) {
  string status_cat;
  long ram;
  string line;
  string spid = std::to_string(pid) + "/";
  std::ifstream stream(kProcDirectory + spid + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> status_cat;
      if (status_cat == "VmSize:") {
        linestream >> ram;
        ram = ram / 1000;
        return std::to_string(ram);
      }
    }
  }
  return std::to_string(ram);
}


string LinuxParser::Uid(int pid) {
  string status_cat, uid;
  string line;
  string spid = std::to_string(pid) + "/";
  std::ifstream stream(kProcDirectory + spid + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> status_cat;
      if (status_cat == "Uid:") {
        linestream >> uid;
        return uid;
      }
    }
  }
  return uid;
}


string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string user, x, user_uid;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> user_uid;
      if (user_uid == uid) {
        return user;
      }
    }
  }
  return user;
}


long LinuxParser::UpTime(int pid) {
  string other_data;
  long uptime;
  string line;
  string spid = std::to_string(pid) + "/";
  std::ifstream stream(kProcDirectory + spid + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; i++) {
      linestream >> other_data;
    }
    linestream >> uptime;
    uptime = uptime / sysconf(_SC_CLK_TCK);
  }
  return uptime;
}
