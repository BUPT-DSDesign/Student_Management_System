package utils

import (
	"math/rand"
	"sync"
	"time"
)

type Snowflake struct {
	machineID uint16
	sequence  uint16
	lastTime  int64
	lock      sync.Mutex
}

const (
	t0          int64  = 1609459200 // 2021-01-01 00:00:00 UTC
	machineMax  uint16 = 0x1FFF     // 13 bits
	sequenceMax uint16 = 0x3FF      // 10 bits
)

func NewSnowflake() *Snowflake {
	rand.Seed(time.Now().UnixNano())
	return &Snowflake{
		machineID: uint16(rand.Intn(int(machineMax))),
		sequence:  0,
		lastTime:  0,
		lock:      sync.Mutex{},
	}
}

func (sf *Snowflake) NextID() int64 {
	sf.lock.Lock()
	defer sf.lock.Unlock()
	timestamp := time.Now().Unix() - t0
	if timestamp < sf.lastTime {
		panic("Time moved backwards")
	}
	if timestamp == sf.lastTime {
		sf.sequence = (sf.sequence + 1) & sequenceMax
		if sf.sequence == 0 {
			time.Sleep(time.Millisecond)
			timestamp = time.Now().Unix() - t0
		}
	} else {
		sf.sequence = 0
	}
	sf.lastTime = timestamp
	return (timestamp << 23) | (int64(sf.machineID) << 10) | int64(sf.sequence)
}

func GenerateId() (int64, error) {
	snowflake := NewSnowflake()
	return snowflake.NextID(), nil
}
