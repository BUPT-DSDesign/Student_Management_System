import { service } from '@/utils/request'

// 日志信息接口
export const LogInfo = () => {
    return service({
        url: '/log/info',
        method: 'get'
    })
}

// 删除日志
export const LogDelete = (logId) => {
    const url = 'log/delete?log_id=' + logId
    return service({
        url: url,
        method: 'delete'
    })
}

