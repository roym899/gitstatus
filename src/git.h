// Copyright 2019 Roman Perepelitsa.
//
// This file is part of GitStatus.
//
// GitStatus is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GitStatus is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GitStatus. If not, see <https://www.gnu.org/licenses/>.

#ifndef ROMKATV_GITSTATUS_GIT_H_
#define ROMKATV_GITSTATUS_GIT_H_

#include <git2.h>

#include <cstddef>
#include <memory>
#include <string>

namespace gitstatus {

// Not null.
const char* GitError();

// Not null.
const char* RepoState(git_repository* repo);

// Returns the number of commits in the range.
size_t CountRange(git_repository* repo, const std::string& range);

// Finds and opens a repo from the specified directory. Returns null if not found.
git_repository* OpenRepo(const std::string& dir);

// How many stashes are there?
size_t NumStashes(git_repository* repo);

// Returns the origin URL or an empty string. Not null.
std::string RemoteUrl(git_repository* repo, const git_reference* ref);

// Returns reference to HEAD or null if not found. The reference is symbolic if the repo is empty
// and direct otherwise.
git_reference* Head(git_repository* repo);

// Returns the name of the local branch, or an empty string.
const char* LocalBranchName(const git_reference* ref);

struct Remote {
  // Tip of the remote branch.
  git_reference* ref;

  // Name of the tracking remote. For example, "origin".
  std::string name;

  // Name of the tracking remote branch. For example, "master".
  std::string branch;

  // URL of the tracking remote. For example, "https://foo.com/repo.git".
  std::string url;

  struct Free {
    void operator()(const Remote* p) const {
      if (p) {
        if (p->ref) git_reference_free(p->ref);
        delete p;
      }
    }
  };
};

using RemotePtr = std::unique_ptr<Remote, Remote::Free>;

RemotePtr GetRemote(git_repository* repo, const git_reference* local);

}  // namespace gitstatus

#endif  // ROMKATV_GITSTATUS_GIT_H_
