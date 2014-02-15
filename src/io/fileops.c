#include "moar.h"
#include "platform/io.h"

#ifndef _WIN32
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#define DEFAULT_MODE 0x0FFF
#else
#include <fcntl.h>
#define O_CREAT  _O_CREAT
#define O_RDONLY _O_RDONLY
#define O_WRONLY _O_WRONLY
#define O_TRUNC  _O_TRUNC
#define DEFAULT_MODE _S_IWRITE /* work around sucky libuv defaults */
#endif

#if MVM_HAS_READLINE
#ifdef __cplusplus
extern "C" {
#endif
    char *readline(const char *);
    void add_history(const char*);
#ifdef __cplusplus
}
#endif
#else
#include <linenoise.h>
#endif

static uv_stat_t file_info(MVMThreadContext *tc, MVMString *filename) {
    char * const a = MVM_string_utf8_encode_C_string(tc, filename);
    uv_fs_t req;

    if (uv_fs_lstat(tc->loop, &req, a, NULL) < 0) {
        free(a);
        MVM_exception_throw_adhoc(tc, "Failed to stat file: %s", uv_strerror(req.result));
    }

    free(a);
    return req.statbuf;
}

MVMint64 MVM_file_stat(MVMThreadContext *tc, MVMString *filename, MVMint64 status) {
    MVMint64 r = -1;

    switch (status) {
        case MVM_stat_exists:             r = MVM_file_exists(tc, filename); break;
        case MVM_stat_filesize:           r = file_info(tc, filename).st_size; break;
        case MVM_stat_isdir:              r = (file_info(tc, filename).st_mode & S_IFMT) == S_IFDIR; break;
        case MVM_stat_isreg:              r = (file_info(tc, filename).st_mode & S_IFMT) == S_IFREG; break;
        case MVM_stat_isdev: {
            const int mode = file_info(tc, filename).st_mode;
#ifdef _WIN32
            r = mode & S_IFMT == S_IFCHR;
#else
            r = (mode & S_IFMT) == S_IFCHR || (mode & S_IFMT) == S_IFBLK;
#endif
            break;
        }
        case MVM_stat_createtime:         r = file_info(tc, filename).st_ctim.tv_sec; break;
        case MVM_stat_accesstime:         r = file_info(tc, filename).st_atim.tv_sec; break;
        case MVM_stat_modifytime:         r = file_info(tc, filename).st_mtim.tv_sec; break;
        case MVM_stat_changetime:         r = file_info(tc, filename).st_ctim.tv_sec; break;
        case MVM_stat_backuptime:         r = -1; break;
        case MVM_stat_uid:                r = file_info(tc, filename).st_uid; break;
        case MVM_stat_gid:                r = file_info(tc, filename).st_gid; break;
        case MVM_stat_islnk:              r = (file_info(tc, filename).st_mode & S_IFMT) == S_IFLNK; break;
        case MVM_stat_platform_dev:       r = file_info(tc, filename).st_dev; break;
        case MVM_stat_platform_inode:     r = file_info(tc, filename).st_ino; break;
        case MVM_stat_platform_mode:      r = file_info(tc, filename).st_mode; break;
        case MVM_stat_platform_nlinks:    r = file_info(tc, filename).st_nlink; break;
        case MVM_stat_platform_devtype:   r = file_info(tc, filename).st_rdev; break;
        case MVM_stat_platform_blocksize: r = file_info(tc, filename).st_blksize; break;
        case MVM_stat_platform_blocks:    r = file_info(tc, filename).st_blocks; break;
        default: break;
    }

    return r;
}

/* copy a file from one to another. */
void MVM_file_copy(MVMThreadContext *tc, MVMString *src, MVMString *dest) {
    uv_fs_t req;
    char *       const a = MVM_string_utf8_encode_C_string(tc, src);
    const uv_file  in_fd = uv_fs_open(tc->loop, &req, (const char *)a, O_RDONLY, 0, NULL);

    if (in_fd >= 0 && uv_fs_stat(tc->loop, &req, a, NULL) >= 0) {
        char *       const b = MVM_string_utf8_encode_C_string(tc, dest);
        const uv_file out_fd = uv_fs_open(tc->loop, &req, (const char *)b, O_WRONLY | O_CREAT | O_TRUNC, DEFAULT_MODE, NULL);
        free(a);

        if (out_fd >= 0
        && uv_fs_sendfile(tc->loop, &req, out_fd, in_fd, 0, req.statbuf.st_size, NULL) >= 0) {
            free(b);

            if (uv_fs_close(tc->loop, &req, in_fd, NULL) < 0) {
                uv_fs_close(tc->loop, &req, out_fd, NULL); /* should close out_fd before throw. */
                MVM_exception_throw_adhoc(tc, "Failed to close file: %s", uv_strerror(req.result));
            }

            if (uv_fs_close(tc->loop, &req, out_fd, NULL) < 0) {
                MVM_exception_throw_adhoc(tc, "Failed to close file: %s", uv_strerror(req.result));
            }

            return;
        }
        else
            free(b);
    }
    else
        free(a);

    MVM_exception_throw_adhoc(tc, "Failed to copy file: %s", uv_strerror(req.result));
}

/* rename one file to another. */
void MVM_file_rename(MVMThreadContext *tc, MVMString *src, MVMString *dest) {
    char * const a = MVM_string_utf8_encode_C_string(tc, src);
    char * const b = MVM_string_utf8_encode_C_string(tc, dest);
    uv_fs_t req;

    if(uv_fs_rename(tc->loop, &req, a, b, NULL) < 0 ) {
        free(a);
        free(b);
        MVM_exception_throw_adhoc(tc, "Failed to rename file: %s", uv_strerror(req.result));
    }

    free(a);
    free(b);
}

void MVM_file_delete(MVMThreadContext *tc, MVMString *f) {
    uv_fs_t req;
    char * const a = MVM_string_utf8_encode_C_string(tc, f);
    const int    r = uv_fs_unlink(tc->loop, &req, a, NULL);

    if( r < 0 && r != UV_ENOENT) {
        free(a);
        MVM_exception_throw_adhoc(tc, "Failed to delete file: %s", uv_strerror(req.result));
    }

    free(a);
}

void MVM_file_chmod(MVMThreadContext *tc, MVMString *f, MVMint64 flag) {
    char * const a = MVM_string_utf8_encode_C_string(tc, f);
    uv_fs_t req;

    if(uv_fs_chmod(tc->loop, &req, a, flag, NULL) < 0 ) {
        free(a);
        MVM_exception_throw_adhoc(tc, "Failed to set permissions on path: %s", uv_strerror(req.result));
    }

    free(a);
}

MVMint64 MVM_file_exists(MVMThreadContext *tc, MVMString *f) {
    uv_fs_t req;
    char * const a = MVM_string_utf8_encode_C_string(tc, f);
    const MVMint64 result = uv_fs_stat(tc->loop, &req, a, NULL) < 0 ? 0 : 1;

    free(a);

    return result;
}

#ifdef _WIN32
#define FILE_IS(name, rwx) \
    MVMint64 MVM_file_is ## name (MVMThreadContext *tc, MVMString *filename) { \
        if (!MVM_file_exists(tc, filename)) \
            return 0; \
        else { \
            uv_stat_t statbuf = file_info(tc, filename); \
            MVMint64 r = (statbuf.st_mode & S_I ## rwx ); \
            return r ? 1 : 0; \
        } \
    }
FILE_IS(readable, READ)
FILE_IS(writable, WRITE)
MVMint64 MVM_file_isexecutable(MVMThreadContext *tc, MVMString *filename) {
    if (!MVM_file_exists(tc, filename))
        return 0;
    else {
        MVMint64 r = 0;
        uv_stat_t statbuf = file_info(tc, filename);
        if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
            return 1;
        else {
            // true if fileext is in PATHEXT=.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC
            MVMString *dot = MVM_string_ascii_decode_nt(tc, tc->instance->VMString, ".");
            MVMROOT(tc, dot, {
                MVMint64 n = MVM_string_index_from_end(tc, filename, dot, 0);
                if (n >= 0) {
                    MVMString *fileext = MVM_string_substring(tc, filename, n, -1);
                    MVMROOT(tc, fileext, {
                        char *ext  = MVM_string_utf8_encode_C_string(tc, fileext);
                        char *pext = getenv("PATHEXT");
                        int plen   = strlen(pext);
                        int i;
                        for (i = 0; i < plen; i++) {
                            if (0 == stricmp(ext, pext++)) {
                                r = 1;
                                break;
                            }
                        }
                        free(ext);
                        free(pext);
                    });
                }
            });
        }
        return r;
    }
}
#else
#define FILE_IS(name, rwx) \
    MVMint64 MVM_file_is ## name (MVMThreadContext *tc, MVMString *filename) { \
        if (!MVM_file_exists(tc, filename)) \
            return 0; \
        else { \
            uv_stat_t statbuf = file_info(tc, filename); \
            MVMint64 r = (statbuf.st_mode & S_I ## rwx ## OTH) \
                      || (statbuf.st_uid == geteuid() && (statbuf.st_mode & S_I ## rwx ## USR)) \
                      || (statbuf.st_uid == getegid() && (statbuf.st_mode & S_I ## rwx ## GRP)); \
            return r ? 1 : 0; \
        } \
    }
FILE_IS(readable, R)
FILE_IS(writable, W)
FILE_IS(executable, X)
#endif

void MVM_file_close_fh(MVMThreadContext *tc, MVMObject *oshandle) {
    MVMOSHandle *handle;
    uv_fs_t req;

    if (((MVMOSHandle *)oshandle)->body.ops) {
        handle = (MVMOSHandle *)oshandle;
        if (handle->body.ops->closable) {
            handle->body.ops->closable->close(tc, handle);
        }
        else {
            MVM_exception_throw_adhoc(tc, "Cannot close this kind of handle");
        }
        return;
    }

    if (handle->body.type == MVM_OSHANDLE_PIPE) {
        if (uv_is_closing((uv_handle_t*)handle->body.u.handle)) {
            return;
        }
        /* closing the in-/output std filehandle will shutdown the child process. */
        uv_unref((uv_handle_t*)handle->body.u.handle);
        uv_close((uv_handle_t*)handle->body.u.handle, NULL);
        uv_run(tc->loop, UV_RUN_DEFAULT);
        if (handle->body.u.process)
#ifdef _WIN32
            uv_process_close(tc->loop, handle->body.u.process);
#else
            waitpid(handle->body.u.process->pid, NULL, 0);
#endif
        uv_unref((uv_handle_t *)handle->body.u.process);
        uv_run(tc->loop, UV_RUN_DEFAULT);
        handle->body.u.process = NULL;
    }
}

/* Reads a line from a filehandle. */
MVMString * MVM_file_readline_fh(MVMThreadContext *tc, MVMObject *oshandle) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->sync_readable) {
        return handle->body.ops->sync_readable->read_line(tc, handle);
    }
    else {
        MVM_exception_throw_adhoc(tc, "Cannot read lines from this kind of handle");
    }
}

/* reads a line from a filehandle. */
MVMString * MVM_file_readline_interactive_fh(MVMThreadContext *tc, MVMObject *oshandle, MVMString *prompt) {
    MVMString *return_str = NULL;
    char *line;
    char * const prompt_str = MVM_string_utf8_encode_C_string(tc, prompt);

#if MVM_HAS_READLINE
    line = readline(prompt_str);

    free(prompt_str);

    if (line) {
        if (*line)
            add_history(line);

        return_str = MVM_string_decode(tc, tc->instance->VMString, line, strlen(line), MVM_encoding_type_utf8);

        free(line);
    }

#else /* !MVM_HAS_READLINE */
    line = linenoise(prompt_str);

    free(prompt_str);

    if (line) {
        if (*line) {
            linenoiseHistoryAdd(line);
        }

        return_str = MVM_string_decode(tc, tc->instance->VMString, line, strlen(line), MVM_encoding_type_utf8);

        free(line);
    }
#endif /* MVM_HAS_READLINE */

    return return_str;
}

/* reads a string from a filehandle. */
MVMString * MVM_file_read_fhs(MVMThreadContext *tc, MVMObject *oshandle, MVMint64 chars) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->sync_readable)
        return handle->body.ops->sync_readable->read_chars(tc, handle, chars);
    else
        MVM_exception_throw_adhoc(tc, "Cannot read charcaters this kind of handle");
}

/* reads a buf from a filehandle. */
void MVM_file_read_fhb(MVMThreadContext *tc, MVMObject *oshandle, MVMObject *result, MVMint64 length) {
    MVMOSHandle *handle;
    MVMint64 bytes_read;
    uv_fs_t req;
    char *buf;

    /* Ensure the target is in the correct form. */
    if (!IS_CONCRETE(result) || REPR(result)->ID != MVM_REPR_ID_MVMArray)
        MVM_exception_throw_adhoc(tc, "read_fhb requires a native array to write to");
    if (((MVMArrayREPRData *)STABLE(result)->REPR_data)->slot_type != MVM_ARRAY_U8
        && ((MVMArrayREPRData *)STABLE(result)->REPR_data)->slot_type != MVM_ARRAY_I8)
        MVM_exception_throw_adhoc(tc, "read_fhb requires a native array of uint8 or int8");

    if (length < 1 || length > 99999999) {
        MVM_exception_throw_adhoc(tc, "read from filehandle length out of range");
    }

    handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->sync_readable)
        bytes_read = handle->body.ops->sync_readable->read_bytes(tc, handle, &buf, length);
    else
        MVM_exception_throw_adhoc(tc, "Cannot read charcaters this kind of handle");

    /* Stash the data in the VMArray. */
    ((MVMArray *)result)->body.slots.i8 = (MVMint8 *)buf;
    ((MVMArray *)result)->body.start    = 0;
    ((MVMArray *)result)->body.ssize    = bytes_read;
    ((MVMArray *)result)->body.elems    = bytes_read;
}

/* Read all of a filehandle into a string. */
MVMString * MVM_file_readall_fh(MVMThreadContext *tc, MVMObject *oshandle) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->sync_readable) {
        return handle->body.ops->sync_readable->slurp(tc, handle);
    }
    else {
        MVM_exception_throw_adhoc(tc, "Cannot slurp this kind of handle");
    }
}

/* read all of a file into a string */
MVMString * MVM_file_slurp(MVMThreadContext *tc, MVMString *filename, MVMString *encoding) {
    MVMString *mode = MVM_string_utf8_decode(tc, tc->instance->VMString, "r", 1);
    MVMObject *oshandle = (MVMObject *)MVM_file_open_fh(tc, filename, mode);
    MVMString *result;
    MVM_file_set_encoding(tc, oshandle, encoding);
    result = MVM_file_readall_fh(tc, oshandle);
    MVM_file_close_fh(tc, oshandle);
    return result;
}

static void write_cb(uv_write_t* req, int status) {
    uv_unref((uv_handle_t *)req);
    free(req);
}

/* writes a string to a filehandle. */
MVMint64 MVM_file_write_fhs(MVMThreadContext *tc, MVMObject *oshandle, MVMString *str, MVMint8 addnl) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (str == NULL)
        MVM_exception_throw_adhoc(tc, "Failed to write to filehandle: NULL string given");
    if (handle->body.ops->sync_writable) {
        return handle->body.ops->sync_writable->write_str(tc, handle, str, addnl);
    }
    else {
        MVM_exception_throw_adhoc(tc, "Cannot write a string to this kind of handle");
    }
}

void MVM_file_write_fhb(MVMThreadContext *tc, MVMObject *oshandle, MVMObject *buffer) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    MVMuint8 *output;
    MVMint64 output_size;
    MVMint64 bytes_written;

    /* Ensure the target is in the correct form. */
    if (!IS_CONCRETE(buffer) || REPR(buffer)->ID != MVM_REPR_ID_MVMArray)
        MVM_exception_throw_adhoc(tc, "write_fhb requires a native array to read from");
    if (((MVMArrayREPRData *)STABLE(buffer)->REPR_data)->slot_type != MVM_ARRAY_U8
        && ((MVMArrayREPRData *)STABLE(buffer)->REPR_data)->slot_type != MVM_ARRAY_I8)
        MVM_exception_throw_adhoc(tc, "write_fhb requires a native array of uint8 or int8");

    output = ((MVMArray *)buffer)->body.slots.i8 + ((MVMArray *)buffer)->body.start;
    output_size = ((MVMArray *)buffer)->body.elems;

    if (handle->body.ops->sync_writable)
        bytes_written = handle->body.ops->sync_writable->write_bytes(tc, handle, output, output_size);
    else
        MVM_exception_throw_adhoc(tc, "Cannot write bytes to this kind of handle");
}

/* writes a string to a file, overwriting it if necessary */
void MVM_file_spew(MVMThreadContext *tc, MVMString *output, MVMString *filename, MVMString *encoding) {
    MVMString *mode = MVM_string_utf8_decode(tc, tc->instance->VMString, "w", 1);
    MVMObject *fh = MVM_file_open_fh(tc, filename, mode);
    MVM_file_set_encoding(tc, fh, encoding);
    MVM_file_write_fhs(tc, fh, output, 0);
    MVM_file_close_fh(tc, fh);
}

/* seeks in a filehandle */
void MVM_file_seek(MVMThreadContext *tc, MVMObject *oshandle, MVMint64 offset, MVMint64 flag) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->seekable)
        handle->body.ops->seekable->seek(tc, handle, offset, flag);
    else
        MVM_exception_throw_adhoc(tc, "Cannot seek this kind of handle");
}

/* tells position within file */
MVMint64 MVM_file_tell_fh(MVMThreadContext *tc, MVMObject *oshandle) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->seekable)
        return handle->body.ops->seekable->tell(tc, handle);
    else
        MVM_exception_throw_adhoc(tc, "Cannot tell this kind of handle");
}

/* Locks a filehandle */
MVMint64 MVM_file_lock(MVMThreadContext *tc, MVMObject *oshandle, MVMint64 flag) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->lockable)
        return handle->body.ops->lockable->lock(tc, handle, flag);
    else
        MVM_exception_throw_adhoc(tc, "Cannot lock this kind of handle");
}

/* Unlocks a filehandle */
void MVM_file_unlock(MVMThreadContext *tc, MVMObject *oshandle) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->lockable)
        handle->body.ops->lockable->unlock(tc, handle);
    else
        MVM_exception_throw_adhoc(tc, "Cannot unlock this kind of handle");
}

/* Syncs a filehandle (Transfer all file modified data and metadata to disk.) */
void MVM_file_sync(MVMThreadContext *tc, MVMObject *oshandle) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->sync_writable)
        handle->body.ops->sync_writable->flush(tc, handle);
    else
        MVM_exception_throw_adhoc(tc, "Cannot flush this kind of handle");
}

/* Truncates a file handle to the specified length. */
void MVM_file_truncate(MVMThreadContext *tc, MVMObject *oshandle, MVMint64 offset) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->sync_writable)
        handle->body.ops->sync_writable->truncate(tc, handle, offset);
    else
        MVM_exception_throw_adhoc(tc, "Cannot truncate this kind of handle");
}

/* return an OSHandle representing one of the standard streams */
MVMObject * MVM_file_get_stdstream(MVMThreadContext *tc, MVMuint8 type, MVMuint8 readable) {
    switch(uv_guess_handle(type)) {
        case UV_TTY: {
            uv_tty_t * const handle = malloc(sizeof(uv_tty_t));
            uv_tty_init(tc->loop, handle, type, readable);
#ifdef _WIN32
            uv_stream_set_blocking((uv_stream_t *)handle, 1);
#else
            ((uv_stream_t *)handle)->flags = 0x80; /* UV_STREAM_BLOCKING */
#endif
            return MVM_io_syncstream_from_uvstream(tc, (uv_stream_t *)handle);
        }
        case UV_FILE:
            return MVM_file_handle_from_fd(tc, type);
        case UV_NAMED_PIPE: {
            uv_pipe_t * const handle = malloc(sizeof(uv_pipe_t));
            uv_pipe_init(tc->loop, handle, 0);
#ifdef _WIN32
            uv_stream_set_blocking((uv_stream_t *)handle, 1);
#else
            ((uv_stream_t *)handle)->flags = 0x80; /* UV_STREAM_BLOCKING */
#endif
            uv_pipe_open(handle, type);
            return MVM_io_syncstream_from_uvstream(tc, (uv_stream_t *)handle);
        }
        default:
            MVM_exception_throw_adhoc(tc, "get_stream failed, unsupported std handle");
    }
}

MVMint64 MVM_file_eof(MVMThreadContext *tc, MVMObject *oshandle) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    if (handle->body.ops->sync_readable) {
        return handle->body.ops->sync_readable->eof(tc, handle);
    }
    else {
        MVM_exception_throw_adhoc(tc, "Cannot eof this kind of handle");
    }
}

void MVM_file_set_encoding(MVMThreadContext *tc, MVMObject *oshandle, MVMString *encoding_name) {
    MVMOSHandle *handle = (MVMOSHandle *)oshandle;
    MVMROOT(tc, oshandle, {
        const MVMuint8 encoding_flag = MVM_string_find_encoding(tc, encoding_name);
        if (handle->body.ops->encodable) {
            handle->body.ops->encodable->set_encoding(tc, handle, encoding_flag);
        }
        else {
            MVM_exception_throw_adhoc(tc, "Cannot set encoding on this kind of handle");
        }
    });
}

/* Takes a filename and prepends any --libpath value we have, if it's not an
 * absolute path. */
MVMString * MVM_file_in_libpath(MVMThreadContext *tc, MVMString *orig) {
    const char **lib_path = tc->instance->lib_path;
    MVM_gc_root_temp_push(tc, (MVMCollectable **)&orig);
    if (lib_path) {
        /* We actually have a lib_path to consider. See if the filename is
         * absolute (XXX wants a platform abstraction, and doing better). */
        char *orig_cstr = MVM_string_utf8_encode_C_string(tc, orig);
        int  absolute   = orig_cstr[0] == '/' || orig_cstr[0] == '\\' ||
                          orig_cstr[1] == ':' && orig_cstr[2] == '\\';
        if (absolute) {
            /* Nothing more to do; we have an absolute path. */
            free(orig_cstr);
            MVM_gc_root_temp_pop(tc); /* orig */
            return orig;
        }
        else {
            MVMString *result = NULL;
            int lib_path_i = 0;
            MVM_gc_root_temp_push(tc, (MVMCollectable **)&result);
            while (lib_path[lib_path_i]) {
                /* Concatenate libpath with filename. */
                size_t lib_path_len = strlen(lib_path[lib_path_i]);
                size_t orig_len     = strlen(orig_cstr);
                int    need_sep     = lib_path[lib_path_i][lib_path_len - 1] != '/' &&
                                      lib_path[lib_path_i][lib_path_len - 1] != '\\';
                int    new_len      = lib_path_len + (need_sep ? 1 : 0) + orig_len;
                char * new_path     = malloc(new_len);
                memcpy(new_path, lib_path[lib_path_i], lib_path_len);
                if (need_sep) {
                    new_path[lib_path_len] = '/';
                    memcpy(new_path + lib_path_len + 1, orig_cstr, orig_len);
                }
                else {
                    memcpy(new_path + lib_path_len, orig_cstr, orig_len);
                }
                result = MVM_string_utf8_decode(tc, tc->instance->VMString, new_path, new_len);
                free(new_path);
                if (!MVM_file_exists(tc, result))
                    result = orig;
                else {
                    MVM_gc_root_temp_pop_n(tc, 2); /* orig and result */
                    return result;
                }
                lib_path_i++;
            }
            if (!result || !MVM_file_exists(tc, result))
                result = orig;
            MVM_gc_root_temp_pop_n(tc, 2); /* orig and result */
            return result;
        }
    }
    else {
        /* No libpath, so just hand back the original name. */
        MVM_gc_root_temp_pop(tc); /* orig */
        return orig;
    }
}

void MVM_file_link(MVMThreadContext *tc, MVMString *oldpath, MVMString *newpath) {
    uv_fs_t req;
    char * const oldpath_s = MVM_string_utf8_encode_C_string(tc, oldpath);
    char * const newpath_s = MVM_string_utf8_encode_C_string(tc, newpath);

    if (uv_fs_link(tc->loop, &req, oldpath_s, newpath_s, NULL)) {
        free(oldpath_s);
        free(newpath_s);
        MVM_exception_throw_adhoc(tc, "Failed to link file: %s", uv_strerror(req.result));
    }

    free(oldpath_s);
    free(newpath_s);
}

void MVM_file_symlink(MVMThreadContext *tc, MVMString *oldpath, MVMString *newpath) {
    uv_fs_t req;
    char * const oldpath_s = MVM_string_utf8_encode_C_string(tc, oldpath);
    char * const newpath_s = MVM_string_utf8_encode_C_string(tc, newpath);

    if (uv_fs_symlink(tc->loop, &req, oldpath_s, newpath_s, 0, NULL)) {
        free(oldpath_s);
        free(newpath_s);
        MVM_exception_throw_adhoc(tc, "Failed to symlink file: %s", uv_strerror(req.result));
    }

    free(oldpath_s);
    free(newpath_s);
}
