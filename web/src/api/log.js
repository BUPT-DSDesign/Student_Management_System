import { service } from '@/utils/request'

// 日志信息接口
export const LogMessage = () => {
    return service({
        url: '/log/info',
        method: 'get'
    })
}