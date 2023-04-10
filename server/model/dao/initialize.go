package dao

import (
	"bufio"
	"fmt"
	"os/exec"
)

// 届时需要打开的数据库后端可执行文件相对路径
const filename string = "./test"

var stdinWriter *bufio.Writer
var stdoutReader *bufio.Reader

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
	if err := cmd.Start(); err != nil {
		panic(err)
	}
	defer func(cmd *exec.Cmd) {
		err := cmd.Wait()
		if err != nil {
			panic(err)
		}
	}(cmd)
	stdinWriter = bufio.NewWriter(stdin)
	stdoutReader = bufio.NewReader(stdout)
}

func WriteLine(data string) error {
	_, err := fmt.Fprintln(stdinWriter, data)
	return err
}

func ReadLine() ([]byte, error) {
	line, _, err := stdoutReader.ReadLine()
	return line, err
}
