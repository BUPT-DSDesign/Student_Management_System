package middleware

import (
	"github.com/gin-gonic/gin"
	"github.com/unrolled/secure"
	"log"
	"strconv"
)

// TlsHandler 传输层安全协议
func TlsHandler(port int) gin.HandlerFunc {
	return func(c *gin.Context) {
		secureMiddleware := secure.New(secure.Options{
			SSLRedirect: true,
			SSLHost:     ":" + strconv.Itoa(port),
		})
		err := secureMiddleware.Process(c.Writer, c.Request)

		// If there was an error, do not continue.
		if err != nil {
			log.Println(err.Error())
			return
		}

		c.Next()
	}
}
