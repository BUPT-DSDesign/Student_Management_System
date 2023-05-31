package dao

import (
	"bufio"
	"fmt"

	"os/exec"
)

// 届时需要打开的数据库后端可执行文件相对路径
const filename string = "E:\\DSDesign\\Student_Management_System\\server\\test_sql.exe"

type DB struct {
	stdinWriter  *bufio.Writer
	stdoutReader *bufio.Reader
}

var db DB

func init() {
	go func() {
		cmd := exec.Command(filename)
		stdin, err := cmd.StdinPipe()
		if err != nil {
			panic(err)
		}
		stdout, err := cmd.StdoutPipe()
		if err != nil {
			panic(err)
		}
		if err := cmd.Start(); err != nil {
			panic(err)
		}
		defer func(cmd *exec.Cmd) {
			err := cmd.Wait()
			if err != nil {
				panic(err)
			}
		}(cmd)
		db.stdinWriter = bufio.NewWriter(stdin)
		db.stdoutReader = bufio.NewReader(stdout)
	}()

}

// ExecSql 执行sql语句
func (db *DB) ExecSql(sqlStr string) error {
	if err := WriteLine(sqlStr); err != nil {
		return err
	}
	return nil
}

func WriteLine(data string) error {
	_, err := fmt.Fprintln(db.stdinWriter, data)
	return err
}

func ReadLine() ([]byte, error) {
	line, _, err := db.stdoutReader.ReadLine()
	return line, err
}
