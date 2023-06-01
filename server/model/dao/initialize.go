package dao

import (
	"encoding/json"
	"fmt"
	"io"
	"log"
	"os/exec"
	"strings"
	"sync"
)

// 届时需要打开的数据库后端可执行文件相对路径
const filename string = "./test_sql"

type DB struct {
	cmd    *exec.Cmd
	stdin  io.WriteCloser
	stdout io.ReadCloser
	mutex  *sync.Mutex
}

type CorrectType float64

var db *DB

func init() {

	cmd := exec.Command(filename)
	stdin, err := cmd.StdinPipe()
	if err != nil {
		panic(err)
	}
	stdout, err := cmd.StdoutPipe()
	if err != nil {
		panic(err)
	}
	if err = cmd.Start(); err != nil {
		panic(err)
	}

	db = new(DB)
	db.stdin = stdin
	db.stdout = stdout
	db.cmd = cmd
	db.mutex = new(sync.Mutex)

	go func() {
		if err := cmd.Wait(); err != nil {
			log.Printf("command %s failed: %s", filename, err)
		}
	}()
}

// ExecSql 执行sql语句
func (db *DB) ExecSql(sqlStr string) error {
	db.mutex.Lock()
	if err := WriteLine(sqlStr); err != nil {
		return err
	}
	return nil
}

func WriteLine(data string) error {
	// 通过db.stdinWriter将数据写入到标准输入
	data = strings.TrimSpace(data)
	_, err := fmt.Fprintf(db.stdin, "%s\n", data)
	return err
}

func ReadLine() ([]byte, error) {
	defer db.mutex.Unlock()
	result := make(map[string]interface{}, 0)
	if err := json.NewDecoder(db.stdout).Decode(&result); err != nil {
		return nil, err
	}
	// 将map转换为json
	jsonStr, err := json.Marshal(result)
	if err != nil {
		return nil, err
	}
	return jsonStr, nil
}
